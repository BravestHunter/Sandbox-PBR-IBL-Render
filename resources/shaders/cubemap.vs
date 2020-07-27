#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 PROJECTION;
uniform mat4 VIEW;

void main()
{
    WorldPos = aPos;  
    gl_Position =  PROJECTION * VIEW * vec4(WorldPos, 1.0);
}
