#include <SDL2/SDL.h>

static int err(const char *msg);
static void mark_unused(SDL_Surface* img, SDL_Color *colors);
static void print_colors(SDL_Color *colors);

int main(int argc, char **argv)
{
	if(argc<2)
		return err("Please specify a BMP");
	SDL_Surface *img = SDL_LoadBMP(argv[1]);
	if(!img)
		return err(SDL_GetError());
	SDL_Palette *p = img->format->palette;
	if(!p)
		return err("Attempt to load image without color index");

	/* Copy into theoretical palette of 256 */
	SDL_Color colors[256] = {0};
	memcpy(colors, p->colors, p->ncolors * sizeof(SDL_Color));

	mark_unused(img, colors);
	print_colors(colors);

	return 0;
}

static int err(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	return -1;
}

void mark_unused(SDL_Surface *img, SDL_Color *colors) {
	char used[256] = {0};
	void *end = img->pixels + img->w * img->h;
	for(char *col = img->pixels; col != end; ++col) {
		used[*col] = 0xff;
	}
	for(int i=0; i<256; ++i) {
		colors[i].a = used[i];
	}
}

void print_colors(SDL_Color *colors) {
	for(int i=0; i<256; ++i) {
		printf("%08x ", *(unsigned*)(&colors[i]));
		if(i%8==7) puts("");
	}
}
