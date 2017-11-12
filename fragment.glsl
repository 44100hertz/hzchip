#version 120

uniform vec2 win_size;
uniform vec3 bg_color;

void main (void)  
{     
	vec2 color_mod = gl_FragCoord.xy / win_size;
	gl_FragColor = vec4(bg_color * color_mod.xxy, 1.0);
}
