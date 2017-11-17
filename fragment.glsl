#version 130

const uvec2 map_size  = uvec2(32u, 32u);
const uvec2 tile_size = uvec2(8u, 8u);
const uint tile_index_mask = (1u << 8u) - 1u;

uniform vec2 win_size, screen_size;
uniform vec4 palette[256];
uniform uint bitmap[64*8];
uniform uint tilemap[map_size.x * map_size.y];

void main (void)  
{
	// Divide the position input into a number of steps equal to pixel res
	vec2 inv_pos   = vec2(gl_FragCoord.x, win_size.y - gl_FragCoord.y);
	vec2 pixel_pos = floor(inv_pos / win_size * screen_size);
	vec2 pos_quant = pixel_pos / screen_size;

	// Locate tile within map
	uvec2 tile_pos  = uvec2(pixel_pos) / tile_size;
	uint tile_index = uint(tile_pos.x + tile_pos.y * map_size.x);
	uint tile       = tilemap[tile_index];

	// Locate pixel within tile
	uvec2 intile_pos = uvec2(pixel_pos) % tile_size;
	uint intile = uint(intile_pos.x + intile_pos.y * tile_size.x);
	uint tile_area = tile_size.x * tile_size.y;
	uint pixel  = tile * tile_area + intile;

	// Unpack pixel from low-depth data
	const uint bpp = 4u;
	const uint mask = (1u<<bpp)-1u;
	const uint bpi = 32u/bpp; // bits per int
	uint color  = mask & (bitmap[pixel / bpi] >> (bpp * (pixel % bpi)));

	gl_FragColor = palette[color];
}
