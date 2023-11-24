#version 450 core

uniform vec3 uColor;

out vec4 OutColor;

void main() 
{
	OutColor = vec4(uColor, 1.0);
}