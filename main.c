#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>

#include "hz/video.h"

static struct hz_vmem *mem;

static int update_loop()
{
	SDL_Event e;
	SDL_PumpEvents();
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return 0;
		}
	}

	int tile = rand();
	mem->map[(rand()>>16) % (HZ_VMAP_AREA)] = *(struct hz_vtile*)&tile;
	hz_vsync(mem);

	return 1;
}

int main() {
	SDL_Init(0);
	hz_vinit();
	mem = hz_vmem_default();
	hz_vloadbmp(mem, "res/text.bmp", 2);
	while (update_loop()) {}
	hz_vquit();
	SDL_Quit();
}
