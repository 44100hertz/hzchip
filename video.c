#include <assert.h>
#include <epoxy/gl.h>
#include <SDL2/SDL.h>

#include "video.h"

static int draw_loop(void *data);

static SDL_mutex *mem_mutex = 0;
static struct mem mem;
static SDL_Window *window;

void video_draw(void fn (struct mem*)) {
	SDL_LockMutex(mem_mutex);
	fn(&mem);
	SDL_UnlockMutex(mem_mutex);
}

void video_init() {
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
		 "it works",
		 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		 800, 600,
		 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	assert(!mem_mutex);
	mem_mutex = SDL_CreateMutex();

	SDL_CreateThread(draw_loop, "drawing", NULL);
}

static int draw_loop(void *data) {
	for(;;) {
		SDL_LockMutex(mem_mutex);
		SDL_UnlockMutex(mem_mutex);
		SDL_GL_SwapWindow(window);
	}
}
