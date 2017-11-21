#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>

#include "hz/video.h"

static struct hz_vmem mem = {0};
struct hz_vbitmap bmp = {0};

static int update_loop()
{
	SDL_Event e;
	SDL_PumpEvents();
	static int enable = 0;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return 0;
		case SDL_KEYDOWN:
			if(e.key.keysym.scancode == SDL_SCANCODE_Y) {
				enable = 1;
			}
		}
	}

	if(enable) {
		int tile = rand();
		mem.map[(rand()>>16) % (HZ_VMAP_AREA)] = *(struct hz_vtile*)&tile;
		size_t off = mem.bitmap - bmp.bitmap;
		off = (off + 1) % (bmp.size - HZ_VPAGE_SIZE);
		mem.bitmap = bmp.bitmap + off;
	}
	hz_vsync(&mem);

	return 1;
}

int main() {
	SDL_Init(0);
	hz_vinit();
	bmp.bpp = 8;
	hz_vloadbmp(&bmp, "res/text.bmp");
	mem.palette = bmp.palette;
	mem.bitmap  = bmp.bitmap;
	mem.bpp     = bmp.bpp;
	mem.map     = calloc(HZ_VMAP_SIZE, 1);
	while (update_loop()) {}
	hz_vquit();
	SDL_Quit();
}
