//#include <epoxy/gl.h>

#define VIDEO_FLIPX_BIT 1
#define VIDEO_FLIPY_BIT 2

struct video_color {
	GLfloat r, g, b, a;
};

struct video_tile {
	GLubyte flags;
	GLubyte color;
	GLubyte index;
};

struct video_mem {
	union {
		struct video_color palette[256];
		GLfloat palette_raw[256*4];
	};
	union {
		struct video_tile tiles[32*32];
		GLuint tiles_raw[32*32];
	};
	GLuint bitmap[64*8];
};

struct video_mem *const video_mem();
void video_sync();
void video_init(void);
void video_quit(void);
void video_loadbmp(const char *path, unsigned int bpp);
