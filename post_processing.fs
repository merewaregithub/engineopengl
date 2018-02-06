#version 330 core
out vec4 color;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bright;

uniform float blurSizeH;
uniform float blurSizeV;

void main() {    
	vec4 color1 = texture(scene, TexCoords);
	//vec4 color2 = texture(bright, TexCoords);

	//color = color1 + color2;

	vec4 blurColor = vec4(0.0);
	int loop = 2;
    for (int x = -loop; x <= loop; x++)
        for (int y = -loop; y <= loop; y++)
            blurColor += texture(bright, vec2(TexCoords.x + x * blurSizeH, TexCoords.y + y * blurSizeV)) / 10.0;

	color = color1 + blurColor;
}  


    
