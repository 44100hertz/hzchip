//#include <epoxy/gl.h>

enum {
	// Colors
	HZ_VMAX_BPP  = 8,
	HZ_VNUM_COLORS = 1 << HZ_VMAX_BPP,
	HZ_VPAL_SIZE = HZ_VNUM_COLORS * 4,
	HZ_VPAL_INTS = HZ_VPAL_SIZE,
	// Bitmap page
	HZ_VPAGE_BITS = 128*128,
	HZ_VPAGE_SIZE = HZ_VPAGE_BITS / 8,
	HZ_VPAGE_INTS = HZ_VPAGE_SIZE / 4,
	// Individual tiles
	HZ_VTILE_W = 8,
	HZ_VTILE_H = 8,
	HZ_VTILE_AREA = HZ_VTILE_W * HZ_VTILE_H,
	HZ_VTILE_SIZE = HZ_VTILE_AREA / 8,
	// Tilemap; each tile is 16 bits
	HZ_VMAP_W = 32,
	HZ_VMAP_H = 32,
	HZ_VMAP_AREA = HZ_VMAP_W * HZ_VMAP_H,
	HZ_VMAP_SIZE = HZ_VMAP_AREA * 2, // two bytes
	HZ_VMAP_INTS = HZ_VMAP_AREA / 2, // ... is half an int
	// Flags (upper bits of vtile color)
	HZ_VFLAG_FLIPX = 1<<4,
	HZ_VFLAG_FLIPY = 1<<5,
	HZ_VFLAG_UNUSED1 = 1<<6,
	HZ_VFLAG_UNUSED2 = 1<<7,
};

struct hz_vcolor {
	GLbyte r, g, b, a;
};
struct hz_vtile {
	GLubyte color; // Only bottom 4 indices are used; top is flags
	GLubyte index; // Index in tilemap
};

// The video memory. Editing it directly is the intended way here, but do know that these pointers expect to see HZ_VTILE_SIZE, HZ_VMAP_SIZE, or HZ_VCOLOR_SIZE of memory.
// By default, these point to allocated placeholder values.
struct hz_vmem {
	// x and y are scroll position.
	// w and h are viewport size; a size of 0 will be used as 256.
	GLubyte x, y, w, h;
	// Bits per pixel; valid values 1, 2, 4, 8
	GLubyte bpp;
	// The bitmap is laid out so that tiles are continuous.
	// Each tile is 8x8; at 4bpp that's 8 ints (32 bytes) per tile.
	// Tiles are packed into this memory; less bpp = can store more.
	void *bitmap;
	struct hz_vcolor *palette;
	struct hz_vtile *map;
};

struct hz_vbitmap {
	struct hz_vcolor palette[HZ_VNUM_COLORS];
	GLubyte bpp;
	size_t size;
	void *bitmap;
};

void hz_vinit(void);
void hz_vquit(void);
void hz_vsync(struct hz_vmem *mem);
struct hz_vmem *hz_vmem_default();
struct hz_vbitmap *hz_vloadbmp(const char *path, GLubyte bpp);
