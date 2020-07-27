#version 330 core

struct PointLight 
{
    vec3 position;  
    vec3 color;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;
in vec2 TexCoords;
in vec4 VerColor;

uniform bool USE_LIGHTING_SIMULATION;

// material info
uniform bool USE_BASE_COLOR_TEXTURE;
uniform bool USE_METALLIC_ROUGHNESS_TEXTURE;
uniform bool USE_NORMAL_TEXTURE;
uniform bool USE_OCCLUSION_TEXTURE;
uniform bool USE_EMISSIVE_TEXTURE;
uniform vec4 BASE_COLOR;
uniform float METALLICITY;
uniform float ROUGHNESS;
uniform float OCCLUSION;
uniform vec4 EMISSIVE;
uniform sampler2D TEXTURE_BASE_COLOR;
uniform sampler2D TEXTURE_METALLIC_ROUGHNESS;
uniform sampler2D TEXTURE_NORMAL;
uniform sampler2D TEXTURE_OCCLUSION;
uniform sampler2D TEXTURE_EMISSIVE;

// IBL
uniform bool USE_IBL;
uniform samplerCube IRRADIANCE_MAP;
uniform samplerCube PREFILTER_MAP;
uniform sampler2D TEXTURE_BRDF_LUT;

// light sources
uniform PointLight POINT_LIGHTS[2];

// camera position
uniform vec3 VIEW_POSITION;

const float PI = 3.14159265359;
vec3 getNormalFromMap();
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

void main()
{
    if (VerColor.a > 0.0)
    {
        FragColor = VerColor;
    }
    else
    {
        vec4 emissive = USE_EMISSIVE_TEXTURE ? texture(TEXTURE_EMISSIVE, TexCoords) : EMISSIVE;

        if (emissive.r > 0.1 || emissive.g > 0.1 || emissive.b > 0.1)
        {
            FragColor = emissive;
        }
        else
        {
            vec3 color = USE_BASE_COLOR_TEXTURE ? texture(TEXTURE_BASE_COLOR, TexCoords).rgb : BASE_COLOR.rgb;
            float alpha = USE_BASE_COLOR_TEXTURE ? texture(TEXTURE_BASE_COLOR, TexCoords).a : BASE_COLOR.a;
            //if (alpha < 0.1)
            //    discard;

            if (!USE_LIGHTING_SIMULATION)
            {
                FragColor = vec4(color, 1.0);
            }
            else
            {
                float metallicity = USE_METALLIC_ROUGHNESS_TEXTURE ? texture(TEXTURE_METALLIC_ROUGHNESS, TexCoords).b : METALLICITY;
                float roughness = USE_METALLIC_ROUGHNESS_TEXTURE ? texture(TEXTURE_METALLIC_ROUGHNESS, TexCoords).g : ROUGHNESS;
                float occlusion = USE_OCCLUSION_TEXTURE ? texture(TEXTURE_OCCLUSION, TexCoords).r : OCCLUSION;
              
                vec3 N = USE_NORMAL_TEXTURE ? getNormalFromMap() : normalize(Normal);
                //N = normalize(Normal);

                vec3 V = normalize(VIEW_POSITION - FragPos);
                vec3 R = reflect(-V, N); 

                // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
                // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
                vec3 F0 = vec3(0.04); 
                F0 = mix(F0, color, metallicity);

                // reflectance equation
                vec3 Lo = vec3(0.0);
                for(int i = 0; i < 2; i++) 
                {
                    // calculate per-light radiance
                    vec3 L = normalize(POINT_LIGHTS[i].position - FragPos);
                    vec3 H = normalize(V + L);
                    float distance = length(POINT_LIGHTS[i].position - FragPos);
                    float attenuation = 1.0 / (distance * distance);
                    vec3 radiance = POINT_LIGHTS[i].color * attenuation;

                    // Cook-Torrance BRDF
                    float NDF = DistributionGGX(N, H, roughness);
                    float G = GeometrySmith(N, V, L, roughness);
                    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

                    vec3 nominator = NDF * G * F;
                    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
                    vec3 specular = nominator / denominator;

                    // kS is equal to Fresnel
                    vec3 kS = F;
                    // for energy conservation, the diffuse and specular light can't
                    // be above 1.0 (unless the surface emits light); to preserve this
                    // relationship the diffuse component (kD) should equal 1.0 - kS.
                    vec3 kD = vec3(1.0) - kS;
                    // multiply kD by the inverse metalness such that only non-metals 
                    // have diffuse lighting, or a linear blend if partly metal (pure metals
                    // have no diffuse light).
                    kD *= 1.0 - metallicity;
                    
                    // scale light by NdotL
                    float NdotL = max(dot(N, L), 0.0);

                    // add to outgoing radiance Lo
                    Lo += (kD * color / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
                }

                // ambient lighting (we now use IBL as the ambient term)
                vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

                vec3 kS = F;
                vec3 kD = 1.0 - kS;
                kD *= 1.0 - metallicity;

                vec3 irradiance = texture(IRRADIANCE_MAP, N).rgb;
                vec3 diffuse = irradiance * color;

                // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
                const float MAX_REFLECTION_LOD = 4.0;
                vec3 prefilteredColor = textureLod(PREFILTER_MAP, R,  roughness * MAX_REFLECTION_LOD).rgb;    
                vec2 brdf = texture(TEXTURE_BRDF_LUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
                vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

                vec3 ambient = (kD * diffuse + specular) * occlusion;

                vec3 visible_color = ambient + Lo;

                // HDR tonemapping
                visible_color = visible_color / (visible_color + vec3(1.0));

                //FragColor = vec4(visible_color, 1.0);
                FragColor = vec4(visible_color, alpha);
            }
        }

        float gamma = 2.2;
        FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
    }
}

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(TEXTURE_NORMAL, TexCoords).xyz * 2.0 - 1.0;

    //vec3 Q1  = dFdx(FragPos);
    //vec3 Q2  = dFdy(FragPos);
    //vec2 st1 = dFdx(TexCoords);
    //vec2 st2 = dFdy(TexCoords);
    //
    //vec3 N = normalize(Normal);
    //vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    //vec3 B = -normalize(cross(N, T));
    //mat3 TBN = mat3(T, B, N);

    vec3 N = normalize(Normal);
    vec3 T = normalize(Tangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a2 = roughness * roughness;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
