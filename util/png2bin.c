#include <png.h>
#include <assert.h>
#include <stdio.h>

#include "../bitmap.h"

static int give_up() {
	puts("png2bin; convert .PNG to a bitmap usable in-engine.");
	puts("PNG's must be 64x64, 256-color indexed");
	puts("Usage: ./bitmap [infile.png] [outfile.bin]");
	return 1;
}

int main(int argc, char **argv) {
	if(argc < 3) {
		return give_up();
	}
	struct bitmap bitmap = {0};

	// TODO: Read PNG into struct

	FILE *file_out = fopen(argv[2], "w");
	size_t len = fwrite(&bitmap, 1, sizeof(bitmap), file_out);
	printf("%zu\n", len);
	fclose(file_out);
	return 0;
}
