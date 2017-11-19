//#include <epoxy/gl.h>

enum {
	// Tile sizes in pixels
	HZ_VTILE_W = 8,
	HZ_VTILE_H = 8,
	HZ_VTILE_SIZE = HZ_VTILE_W * HZ_VTILE_H,

	// Color
	HZ_VMAX_BPP  = 8,
	HZ_VPAL_SIZE = 1 << HZ_VMAX_BPP,
	HZ_VPAL_INTS = HZ_VPAL_SIZE,

	// Size of bitmap page in bits
	HZ_VPAGE_BITS = 128*128,
	HZ_VPAGE_SIZE = HZ_VPAGE_BITS / 8,
	HZ_VPAGE_INTS = HZ_VPAGE_SIZE / 4,
	HZ_VTILES_PER_PAGE = HZ_VPAGE_BITS / HZ_VTILE_SIZE,

	// Size of tilemap in tiles
	HZ_VMAP_W = 32,
	HZ_VMAP_H = 32,
	HZ_VMAP_AREA = HZ_VMAP_W * HZ_VMAP_H,
	HZ_VMAP_INTS = HZ_VMAP_AREA / 2, // Tiles are 16 bits wide, half an int

	// Video flags (upper bits of color)
	HZ_VFLAG_FLIPX = 1<<4,
	HZ_VFLAG_FLIPY = 1<<5,
	HZ_VFLAG_UNUSED1 = 1<<6,
	HZ_VFLAG_UNUSED2 = 1<<7,
};

struct hz_vtile {
	GLubyte color; // Only bottom 4 indices are used; top is flags
	GLubyte index; // Index in tilemap
};

struct hz_vcolor {
	GLbyte r, g, b, a;
};

struct hz_vmem {
	struct hz_vcolor palette[HZ_VPAL_SIZE];
	struct hz_vtile tiles[HZ_VMAP_AREA];
	// x and y are scroll position.
	// w and h are viewport size; a size of 0 will be used as 256.
	GLubyte x, y, w, h;
	// The bitmap is laid out so that tiles are continuous.
	// Each tile is 8x8; at 4bpp that's 8 ints (32 bytes) per tile.
	// Tiles are packed into this memory; less bpp = can store more.
	GLuint bitmap[HZ_VPAGE_INTS];
};

void hz_vinit(void);
void hz_vquit(void);

struct hz_vmem *const hz_vmem();
void hz_vsync();
GLuint hz_vloadbmp(const char *path, GLubyte bpp);
