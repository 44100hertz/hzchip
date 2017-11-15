#include <png.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static int give_up() {
	puts("bitmap; convert .PNG to a bitmap usable in-engine.");
	puts("PNG's must be 64x64, 256-color indexed");
	puts("Usage: ./bitmap [infile.png] [outfile.bin]");
	return 1;
}

int main(int argc, char **argv) {
	if(argc < 3) {
		return give_up();
	}
	struct {
		struct { uint8_t r, g, b, a; } pal[256];
		uint8_t tiles[64*64];
	} bitmap = {0};

	// TODO: Read PNG into struct

	FILE *file_out = fopen(argv[2], "w");
	size_t len = fwrite(&bitmap, 1, sizeof(bitmap), file_out);
	printf("%zu\n", len);
	fclose(file_out);
	return 0;
}
