#version 330 core
out vec4 FragColor;

in vec3 localPos;

uniform samplerCube ENVIRONMENT_MAP;

void main()
{
    vec3 envColor = texture(ENVIRONMENT_MAP, localPos).rgb;
    
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(envColor, 1.0);
}
