#version 330 core
out vec4 FragColor;

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
	//FragColor = texture(texture1, TexCoord);
}