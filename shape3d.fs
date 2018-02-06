#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main()
{
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	BrightColor = vec4(vec3(0.0), 1.0);
}