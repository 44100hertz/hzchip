#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>

#include "video.h"

static void rotate_hue(struct video_mem *mem, void *data)
{
	float hue = *(float*)data;
	const float TAU_3 = acos(-1.0) * 2.0 / 3.0;
	mem->bg_col = (struct color){
		sin(hue + TAU_3 * 0.0) / 2.0 + 0.5,
		sin(hue + TAU_3 * 1.0) / 2.0 + 0.5,
		sin(hue + TAU_3 * 2.0) / 2.0 + 0.5,
		1.0,
	};
}

static int update_loop()
{
	SDL_Event e;
	const double frame_len = 1000.0/60.0;
	static double next_frame = 0;

	SDL_PumpEvents();
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return 0;
		}
	}

	float hue = (float)SDL_GetTicks() / 1000.0;
	video_draw(rotate_hue, &hue);

	next_frame += frame_len;
	long ticks = SDL_GetTicks();
	if (next_frame > ticks) {
		SDL_Delay(next_frame - ticks);
	}

	return 1;
}

int main() {
	SDL_Init(0);
	video_init();
	while (update_loop()) {}
	video_quit();
	SDL_Quit();
}
