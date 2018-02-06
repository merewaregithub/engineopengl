#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec4 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

uniform float smoothing;

uniform float width;// for large text -> use high
uniform float edge; // for large text -> use low

void main()
{
	float distance = 1.0 - texture(texture1, TexCoord).a;
	float alpha = 1.0 - smoothstep(width, width + edge, distance);
	FragColor = vec4(ourColor.rgb,alpha);
	BrightColor = vec4(vec3(0.0), 1.0);
	//FragColor = texture(texture1, TexCoord);
}