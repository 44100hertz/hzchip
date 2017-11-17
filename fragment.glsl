#version 130

const uvec2 map_size  = uvec2(32u, 32u);
const uvec2 tile_size = uvec2(8u, 8u);

uniform vec2 win_size;
uniform vec2 viewport = vec2(256, 224);
uniform vec2 scroll;
uniform vec4 palette[256];
uniform uint bitmap[8*8*8];
uniform uint bpp = 4u;
uniform uint tilemap[map_size.x * map_size.y];

// Tile memory layout:
// ____ ____ ____ __YX CCCC CCCC IIII IIII
// XY = flip; C = color; I = index;
// Will only use as many index/color bits as possible given other info.
uint tilemap_get_index(uint tile) { return tile % (256u / bpp); }
uint tilemap_get_color(uint tile) { return (tile >> 16u) & ((1u << bpp) - 1u); }
bool tilemap_get_flipx(uint tile) { return bool(tile & (1u << 20u)); }
bool tilemap_get_flipy(uint tile) { return bool(tile & (1u << 21u)); }

void main (void)  
{
	// Divide the position input into a number of steps equal to pixel res
	vec2 scroll_pos = gl_FragCoord.xy + mod(scroll, map_size*tile_size);
	vec2 inv_pos    = vec2(scroll_pos.x, win_size.y - scroll_pos.y);
	vec2 pixel_pos  = floor(inv_pos / win_size * viewport);
	vec2 pos_quant  = pixel_pos / viewport;

	// Locate tile within map
	uvec2 tile_pos  = uvec2(pixel_pos) / tile_size;
	uint tile_index = uint(tile_pos.x + tile_pos.y * map_size.x);
	uint tile       = tilemap[tile_index];

	// Locate pixel within tile
	uvec2 intile_pos = uvec2(pixel_pos) % tile_size;
	if (tilemap_get_flipx(tile)) {
		intile_pos.x = tile_size.x - intile_pos.x - 1u;
	}
	if (tilemap_get_flipy(tile)) {
		intile_pos.y = tile_size.y - intile_pos.y - 1u;
	}
	uint intile = uint(intile_pos.x + intile_pos.y * tile_size.x);
	uint tile_area = tile_size.x * tile_size.y;
	uint pixel = tilemap_get_index(tile) * tile_area + intile;

	// Unpack pixel from low-depth data
	uint mask = (1u<<bpp)-1u;
	uint bpi = 32u/bpp; // bits per int
	uint color = mask & (bitmap[pixel / bpi] >> (bpp * (pixel % bpi)));
	uint color_off = tilemap_get_color(tile) * 16u;

	gl_FragColor = palette[color + color_off];
}
