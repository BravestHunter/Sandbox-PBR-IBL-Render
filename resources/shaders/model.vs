#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec4 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;
out vec2 TexCoords;
out vec4 VerColor;

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJECTION;

void main()
{
	FragPos = vec3(MODEL * vec4(aPos, 1.0));
	Normal = mat3(MODEL) * aNormal;
	Tangent = normalize(mat3(MODEL) * aTangent.xyz);
	Bitangent = normalize((MODEL * vec4(cross(aNormal, aTangent.xyz), 0.0)).xyz);
    TexCoords = aTexCoords;
    VerColor = aColor;

    gl_Position = PROJECTION * VIEW * MODEL * vec4(aPos, 1.0);
}
