//#include <epoxy/gl.h>

struct mem {
	int window_scale;
	GLfloat bg_col[3];
};

void video_draw(void fn(struct mem*));
void video_init(void);
