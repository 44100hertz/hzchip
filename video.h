//#include <epoxy/gl.h>

struct color {
	GLfloat r, g, b, a;
};

struct video_mem {
	struct color palette[256];
	GLuint bitmap[64*8];
	GLuint tilemap[32*32];
};

struct video_mem *const video_mem();
void video_sync();
void video_init(void);
void video_quit(void);
void video_loadbmp(const char *path);
