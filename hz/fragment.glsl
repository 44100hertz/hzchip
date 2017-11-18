#version 130

const uvec2 map_size  = uvec2(32u, 32u);
const uvec2 tile_size = uvec2(8u, 8u);
const uint  tile_area = tile_size.x * tile_size.y;

uniform vec2 win_size;
uniform vec2 viewport;
uniform vec2 scroll;
uniform vec4 palette[256];
uniform uint bitmap[8*8*8];
uniform uint bpp = 4u;
uniform uint tilemap[32*32/2];

uniform bool vertical;

// Tile memory layout:
// __YX CCCC IIII IIII
// XY = flip; C = color; I = index;
// Will only use as many index/color bits as possible given other info.

uint tilemap_get_index(uint tile) { return tile % (256u / bpp); }
uint tilemap_get_color(uint tile) { return (tile >> 8u) & 15u; }
bool tilemap_get_flipx(uint tile) { return bool(tile & (1u << 12u)); }
bool tilemap_get_flipy(uint tile) { return bool(tile & (1u << 13u)); }

void main (void)  
{
	// Divide the position input into a number of steps equal to pixel res
	vec2 inv_pos    = vec2(gl_FragCoord.x, win_size.y - gl_FragCoord.y);
	vec2 pixel_pos  = floor(inv_pos / win_size * viewport);
	pixel_pos = mod(pixel_pos + scroll, map_size * tile_size);
	vec2 pos_quant  = pixel_pos / viewport;

	// Locate tile within map
	uint t_mask  = (1u << 16) - 1u;
	uvec2 t_pos  = uvec2(pixel_pos) / tile_size;
	uint t_index = uint(t_pos.x + t_pos.y * map_size.x);
	uint tile    = t_mask & (tilemap[t_index/2u] >> (16u * (t_index % 2u)));

	// Locate pixel within tile
	uvec2 intile_pos = uvec2(pixel_pos) % tile_size;
	if (tilemap_get_flipx(tile)) {
		intile_pos.x = tile_size.x - intile_pos.x - 1u;
	}
	if (tilemap_get_flipy(tile)) {
		intile_pos.y = tile_size.y - intile_pos.y - 1u;
	}
	uint intile    = uint(intile_pos.x + intile_pos.y * tile_size.x);
	uint pixel     = tilemap_get_index(tile) * tile_area + intile;

	// Unpack pixel from low-depth data
	uint c_mask  = (1u << bpp) - 1u;
	uint bpi   = 32u/bpp; // bits per int
	uint color = c_mask & (bitmap[pixel / bpi] >> (bpp * (pixel % bpi)));
	uint color_off = (tilemap_get_color(tile) * 16u) & 255u;

	gl_FragColor = palette[color + color_off];
}
