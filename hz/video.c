#include <assert.h>
#include <epoxy/gl.h>
#include <SDL2/SDL.h>

#include "video.h"
#include "file.h"

#define MIN(a, b) a < b ? a : b

static GLuint load_shader(const char *filename, GLenum kind);
static void check_bpp(GLubyte *bpp);

static SDL_Window *window;

static const GLfloat verts[8] = {-1, -1,  1, -1, -1,  1,  1,  1};
static GLuint program, vbo;
static struct {
	GLint win_size, viewport, scroll,
	      palette, bitmap, bpp, tilemap;
} uniform;

void hz_vdraw_tiles(struct hz_vmem *mem)
{
	assert(mem->palette);
	assert(mem->bitmap);
	assert(mem->tilemap);
	check_bpp(&mem->bpp);

	int win_w, win_h;
	SDL_GetWindowSize(window, &win_w, &win_h);
	glViewport(0, 0, win_w, win_h);

	glUseProgram(program);

	glUniform2f  (uniform.win_size, win_w, win_h);
	glUniform2f  (uniform.scroll,   mem->x, mem->y);
	// (x-1&255)+1 is a bit hack to map 0 to 256
	glUniform2f  (uniform.viewport, ((mem->w-1) & 255)+1, ((mem->h-1) & 255)+1);
	glUniform1uiv(uniform.palette,  HZ_VPAL_INTS, (GLuint*)mem->palette);
	glUniform1uiv(uniform.bitmap,   HZ_VPAGE_INTS, mem->bitmap);
	glUniform1uiv(uniform.tilemap,	HZ_VMAP_INTS, (GLuint*)mem->tilemap);
	glUniform1ui (uniform.bpp,	mem->bpp);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void hz_vsync()
{
	SDL_GL_SwapWindow(window);
}

void hz_vinit()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	// Hacks until I get better memory alloc model

	window = SDL_CreateWindow(
		 "it works",
		 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		 512, 512,
		 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	SDL_GL_SetSwapInterval(1);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	int v = epoxy_gl_version();
	printf("Initialized GL Version: %d.%d\n", v/10, v%10);

	program = glCreateProgram();
	GLuint vertex = load_shader("vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragment = load_shader("fragment.glsl", GL_FRAGMENT_SHADER);
	if (!vertex || !fragment) {
		return;
	}
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glUseProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

#define U(name) uniform. name = glGetUniformLocation(program, #name)
	U(win_size);
	U(viewport);
	U(scroll);
	U(palette);
	U(bitmap);
	U(bpp);
	U(tilemap);
#undef U
}

void hz_vquit()
{
}

struct hz_vbitmap hz_vbitmap_new(GLubyte bpp)
{
	check_bpp(&bpp);
	struct hz_vbitmap bitmap;
	memset(&bitmap, 0, sizeof(bitmap));
	bitmap.bpp = bpp;
	return bitmap;
}

void hz_vloadbmp(struct hz_vbitmap* bitmap, const char *path)
{
	const GLbyte bpp = bitmap->bpp;
	SDL_Surface *img = SDL_LoadBMP(path);
	if(!img) {
		fprintf(stderr, "%s", SDL_GetError());
		return;
	}
	SDL_Palette *pal = img->format->palette;
	if(!pal) {
		fprintf(stderr, "Attempt to use non-indexed image\n");
		return;
	}
	GLuint isize = img->w * img->h;
	printf("Loading image %s, size %d tiles, as %dbpp\n",
			path, isize / HZ_VTILE_AREA, bpp);

	size_t oldsize = bitmap->size;
	bitmap->size  += isize * bpp / 8;
	bitmap->bitmap = realloc(bitmap->bitmap, bitmap->size);
	memset(bitmap->bitmap + oldsize, 0, bitmap->size - oldsize);

	for(unsigned i=0; i<isize; ++i) {
		// Find actual x and y
		GLuint x = i % img->w;
		GLuint y = i / img->w;
		// Chop image into 8-tall strips
		x += (y/HZ_VTILE_H) * img->w;
		y %= HZ_VTILE_H;
		// Find position within tile
		GLuint pos = x % HZ_VTILE_W + y * HZ_VTILE_H + (x / HZ_VTILE_W) * HZ_VTILE_AREA;
		// Pack into lower bit depth
		char px = ((char*)img->pixels)[i];
		const GLuint mask = (1<<bpp)-1;
		GLbyte *b = bitmap->bitmap;
		const GLuint bp8 = 8/bpp;
		b[pos / bp8] |= (px & mask) << ((pos % bp8) * bpp);
	}
	for(int i=0; i<256; ++i) {
		SDL_Color c = pal->colors[i];
		bitmap->palette[i] = (struct hz_vcolor){c.r, c.g, c.b, c.a};
	}

	SDL_FreeSurface(img);
}

static GLuint load_shader(const char *filename, GLenum kind)
{
	const char root[] = "hz/";
	char *path = malloc(strlen(root) + strlen(filename) - 1);
	memcpy(path, root, sizeof(root));
	strcat(path, filename);
	const char *source = file_to_string(path);
	free(path);

	GLuint shader = glCreateShader(kind);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	GLchar *log;
	GLint len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len) {
		log = malloc(len);
		glGetShaderInfoLog(shader, len, NULL, log);
		printf("%s:\n\t%s", filename, log);
		free(log);
	}

	return compiled ? shader : 0;
}

static void check_bpp(GLubyte *bpp)
{
	GLubyte b = *bpp;
	if(b == 0) {
		puts("Uninitialized bpp; setting to 4bpp");
		*bpp = 4;
	}
	if((HZ_VMAX_BPP/b)*b != HZ_VMAX_BPP) {
		fprintf(stderr, "Attempt to use depth that does not divide %d evenly.\n"
				"Falling back on 4bpp.\n", HZ_VMAX_BPP);
		*bpp = 4;
	}
}
