//#include <epoxy/gl.h>

struct color {
	GLfloat r, g, b, a;
};

struct video_mem {
	struct color bg_col;
};

void video_draw(void fn(struct video_mem*, void*), void *data);
void video_init(void);
void video_quit(void);
