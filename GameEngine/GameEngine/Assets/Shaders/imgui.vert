#version 330 core

uniform mat4 viewMatrix;
layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec4 Color;

out vec2 Frag_UV;
out vec4 Frag_Color;

void main()
{
	Frag_UV = UV;
	Frag_Color = Color;
	gl_Position = viewMatrix * vec4(Position.x, Position.y, 0, 1);
	gl_Position.w = 1.0f;
}