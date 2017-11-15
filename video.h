//#include <epoxy/gl.h>

struct color {
	GLfloat r, g, b, a;
};

struct video_mem {
	struct color bg_col;
	GLuint bitmap;
};

struct video_mem *const video_mem();
void video_sync();
void video_init(void);
void video_quit(void);
