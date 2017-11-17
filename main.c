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
	mem->tiles_raw[(rand()>>16) % (32*32)] = rand();
	video_sync();

	return 1;
}

int main() {
	SDL_Init(0);
	video_init();
	video_loadbmp("res/text1.bmp");
	while (update_loop()) {}
	video_quit();
	SDL_Quit();
}
