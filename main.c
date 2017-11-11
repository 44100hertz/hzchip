#include <epoxy/gl.h>
#include <SDL2/SDL.h>
#include <stdio.h>

#include "video.h"

static int update_loop() {
	SDL_Event e;
	SDL_PumpEvents();
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			return 0;
		}
	}

	static double next_frame = 0;
	const double frame_len = 1000.0/60.0;
	next_frame += frame_len;
	while(SDL_GetTicks() < (int)next_frame) {
		SDL_Delay(1);
	}

	return 1;
}

int main() {
	SDL_Init(0);
	video_init();
	while(update_loop());
}
