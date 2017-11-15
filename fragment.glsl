#version 120

uniform vec2 win_size, screen_size;
uniform vec3 bg_color;

uniform sampler2D bitmap;

void main (void)  
{     
	// Divide the position input into a number of steps equal to pixel res
//	vec2 pixel_pos = floor(gl_FragCoord.xy / win_size * screen_size);
	vec2 pixel_pos = gl_FragCoord.xy;
	vec2 pos_quant = pixel_pos / screen_size;

	vec2 off_pos = pixel_pos + bg_color.rg * 1000.0;
	float circles = sin(off_pos.x / 10.0) * cos(off_pos.y / 10.0);

//	gl_FragColor = vec4(bg_color * pos_quant.xxy * circles, 1.0);
	gl_FragColor = texture2D(bitmap, pos_quant) + 0.5;
}
