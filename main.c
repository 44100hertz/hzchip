#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>

#include "hz/video.h"
#include "hz/audio.h"

static struct hz_vmem mem;
struct hz_vbitmap bmp;

static void set_amem(struct hz_amem *mem, void *data)
{
	static int note = 40 << 8;
	mem->voices[0].wave = HZ_TRI;
	mem->voices[0].pitch = (note += 20);
	mem->voices[0].vol = 0x80;
}

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
		mem.tilemap[(rand()>>16) % (HZ_VMAP_AREA)] = *(struct hz_vtile*)&tile;
	}
	hz_vdraw_tiles(&mem);
	hz_vsync();
	hz_aupdate(set_amem, 0);

	return 1;
}

int main() {
	SDL_Init(0);
	hz_vinit();
	hz_ainit(60);

	bmp = hz_vbitmap_new(2);
	hz_vloadbmp(&bmp, "res/text.bmp");
	mem.palette = bmp.palette;
	mem.bitmap  = bmp.bitmap;
	mem.bpp     = bmp.bpp;
	mem.tilemap = calloc(HZ_VMAP_SIZE, 1);

	while (update_loop()) {}

	hz_vquit();
	SDL_Quit();
}
