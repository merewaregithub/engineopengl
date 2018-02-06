#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 ourColor;
uniform vec4 color;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(aPos, 1.0);
	ourColor = color;
}