struct bitmap {
	struct bitmap_color {
		char r, g, b, a;
	} pal[256];
	char tiles[64*64];
};
