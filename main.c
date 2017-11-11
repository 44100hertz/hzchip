#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>

#include "video.h"

void rotate_hue(struct mem *mem, void *data) {
	float hue = *(float*)data;
	mem->bg_col[0] = sin(hue + (0.0/3.0)) / 2.0 + 0.5;
	mem->bg_col[1] = sin(hue + (1.0/3.0)) / 2.0 + 0.5;
	mem->bg_col[2] = sin(hue + (2.0/3.0)) / 2.0 + 0.5;
}

static int update_loop() {
	SDL_Event e;
	const double frame_len = 1000.0/60.0;
	static double next_frame = 0;

	SDL_PumpEvents();
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			return 0;
		}
	}

	float hue = (float)SDL_GetTicks() / 1000.0;
	video_draw(rotate_hue, &hue);

	next_frame += frame_len;
	signed delay = next_frame - SDL_GetTicks();
	if(delay > 0) SDL_Delay(delay);

	return 1;
}

int main() {
	SDL_Init(0);
	video_init();
	while(update_loop());
	SDL_Quit();
}
