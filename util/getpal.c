#include <SDL2/SDL.h>

const int err(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	return -1;
}

unsigned col_to_rgb(SDL_Color c) {
	return c.r << 16
		| c.g << 8
		| c.b;
}

int main(int argc, char **argv) {
	if(argc<2)
		return err("Please specify a BMP");
	SDL_Surface *img = SDL_LoadBMP(argv[1]);
	if(!img)
		return err(SDL_GetError());
	SDL_Palette *p = img->format->palette;
	if(!p)
		return err("Attempt to load image without color index");
	for(int i=0; i<p->ncolors; ++i) {
		printf("%06x ", col_to_rgb(p->colors[i]));
		if(i%8==7) puts("");
	}
	return 0;
}
