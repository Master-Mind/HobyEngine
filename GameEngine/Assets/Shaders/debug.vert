#version 330 core

uniform mat4 view;
uniform mat4 projection;
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;

out vec4 Frag_Color;

void main()
{
	Frag_Color = color;
	gl_Position = projection * view * vec4(pos, 1);
}