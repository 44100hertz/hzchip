#version 130

uniform vec2 win_size, screen_size;
uniform vec4 palette[256];
uniform uint bitmap[64*8];

void main (void)  
{     
	// Divide the position input into a number of steps equal to pixel res
	vec2 pixel_pos = floor(gl_FragCoord.xy / win_size * screen_size);
	vec2 pos_quant = pixel_pos / screen_size;

	// 4bpp color algorithm
	uint pos = uint(pixel_pos.x + (screen_size.y - pixel_pos.y)*64);
	uint index = 15u & (bitmap[pos / 8u] >> (4u * (pos % 8u)));

	gl_FragColor = palette[index];
}
