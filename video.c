#include <assert.h>
#include <epoxy/gl.h>
#include <SDL2/SDL.h>

#include "video.h"
#include "file.h"

static int draw_thread(void *data);
static GLuint load_shader(const char *filename);

static SDL_mutex *mem_mutex = 0;
static struct video_mem mem;
static SDL_Window *window;

void video_draw(void fn (struct video_mem*, void*), void *data) {
	SDL_LockMutex(mem_mutex);
	fn(&mem, data);
	SDL_UnlockMutex(mem_mutex);
}

void video_init() {
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	assert(!mem_mutex);
	mem_mutex = SDL_CreateMutex();
	memset(&mem, 0, sizeof(mem));

	SDL_CreateThread(draw_thread, "drawing", NULL);
}

static int draw_thread(void *data) {
	window = SDL_CreateWindow(
		 "it works",
		 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		 800, 600,
		 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);
	SDL_GL_SetSwapInterval(1);

	GLuint program = glCreateProgram();
	GLuint fragment = load_shader("fragment.glsl");
	if(!fragment) return 1;
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glUseProgram(program);

	for (;;) {
		SDL_LockMutex(mem_mutex);

		glClearColor(mem.bg_col.r, mem.bg_col.g, mem.bg_col.b, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_UnlockMutex(mem_mutex);
		SDL_GL_SwapWindow(window);
	}
}

static GLuint load_shader(const char *filename) {
	const char *source = file_to_string(filename);

	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	GLchar *log;
	GLint len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if(len) {
		log = malloc(len);
		glGetShaderInfoLog(shader, len, NULL, log);
		printf("%s:\n\t%s", filename, log);
		free(log);
	}

	return compiled ? shader : 0;
}
