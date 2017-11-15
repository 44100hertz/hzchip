#version 120

uniform vec2 win_size, screen_size;
uniform vec3 bg_color;

uniform sampler2D bitmap;

void main (void)  
{     
	// Divide the position input into a number of steps equal to pixel res
	vec2 pixel_pos = gl_FragCoord.xy / win_size * screen_size;
	vec2 pos_quant = floor(pixel_pos * (bg_color.rg + 0.1) + bg_color.b * 100) / screen_size;

	vec3 color = texture2D(bitmap, pos_quant).rrr * bg_color;
	gl_FragColor = vec4(color, 1.0);
}
