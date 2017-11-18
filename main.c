#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>

#include "video.h"

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

	struct video_mem *mem = video_mem();
	int tile = rand();
	mem->tiles[(rand()>>16) % (32*32)] = *(struct video_tile*)&tile;
	video_sync();

	return 1;
}

int main() {
	SDL_Init(0);
	video_init();
	video_loadbmp("res/text.bmp", 2);
	while (update_loop()) {}
	video_quit();
	SDL_Quit();
}
