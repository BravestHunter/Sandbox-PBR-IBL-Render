#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 PROJECTION;
uniform mat4 VIEW;

out vec3 localPos;

void main()
{
	localPos = aPos;
    vec4 pos = PROJECTION * VIEW * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
