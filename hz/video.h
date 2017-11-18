//#include <epoxy/gl.h>

#define HZ_VFLAG_FLIPX (1<<5)
#define HZ_VFLAG_FLIPY (1<<6)

struct hz_vtile {
	GLubyte  color; // Only bottom 4 indices are used; top is flags
	GLubyte  index; // Index in tilemap
};

struct hz_vcolor {
	GLfloat r, g, b, a;
};

struct hz_vmem {
	struct hz_vcolor palette[256];
	struct hz_vtile tiles[32*32];
	// x and y are scroll position.
	// w and h are viewport size; a size of 0 will be used as 256.
	GLubyte x, y, w, h;
	// The bitmap is laid out so that tiles are continuous.
	// Each tile is 8x8; at 4bpp that's 8 ints (32 bytes) per tile.
	// Tiles are packed into this memory; less bpp = can store more.
	GLuint bitmap[64*8];
};

void hz_vinit(void);
void hz_vquit(void);

struct hz_vmem *const hz_vmem();
void hz_vsync();
void hz_vloadbmp(const char *path, GLuint bpp);
