#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec4 ourColor;

void main()
{
	FragColor = ourColor;
	BrightColor = vec4(vec3(0.0), 1.0);
}