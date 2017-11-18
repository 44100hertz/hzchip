//#include <epoxy/gl.h>

#define VIDEO_FLIPX_BIT 1
#define VIDEO_FLIPY_BIT 2

struct video_color {
	GLfloat r, g, b, a;
};

struct video_tile {
	GLubyte pad;
	GLubyte flags;
	GLubyte color;
	GLubyte index;
};

struct video_mem {
	struct video_color palette[256];
	struct video_tile tiles[32*32];
	unsigned char x, y, w, h;
	GLuint bitmap[64*8];
};

void video_init(void);
void video_quit(void);

struct video_mem *const video_mem();
void video_sync();
void video_loadbmp(const char *path, unsigned int bpp);
