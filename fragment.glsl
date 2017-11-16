#version 120

uniform vec2 win_size, screen_size;
uniform vec4 palette[256];

uniform sampler2D bitmap;

void main (void)  
{     
	// Divide the position input into a number of steps equal to pixel res
	vec2 inv_pos = gl_FragCoord.xy * vec2(1, -1);
	vec2 pixel_pos = inv_pos / win_size * screen_size;
	vec2 pos_quant = floor(pixel_pos) / screen_size;

	int index = int(texture2D(bitmap, pos_quant) * 128.0);
	gl_FragColor = palette[index];
}
