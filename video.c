#include <assert.h>
#include <epoxy/gl.h>
#include <SDL2/SDL.h>

#include "video.h"
#include "file.h"

#define RES_X 240
#define RES_Y 160

static int draw_thread(void *data);
static GLuint load_shader(const char *filename, GLenum kind);

static SDL_mutex *mem_mutex = 0;
static struct video_mem mem;
static SDL_Window *window;

void video_draw(void fn (struct video_mem*, void*), void *data)
{
	SDL_LockMutex(mem_mutex);
	fn(&mem, data);
	SDL_UnlockMutex(mem_mutex);
}

void video_init()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	assert(!mem_mutex);
	mem_mutex = SDL_CreateMutex();
	memset(&mem, 0, sizeof(mem));

	SDL_CreateThread(draw_thread, "drawing", NULL);
}

void video_quit()
{
	SDL_LockMutex(mem_mutex);
	mem.quit = 1;
	SDL_DestroyWindow(window);
	SDL_UnlockMutex(mem_mutex);
}

static int draw_thread(void *data)
{
	window = SDL_CreateWindow(
		 "it works",
		 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		 800, 600,
		 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	SDL_GL_SetSwapInterval(1);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	{
		int v = epoxy_gl_version();
		printf("Initialized GL Version: %d.%d\n", v/10, v%10);
	}

	GLuint program = glCreateProgram();
	GLuint vertex = load_shader("vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragment = load_shader("fragment.glsl", GL_FRAGMENT_SHADER);
	if (!vertex || !fragment) {
		return 1;
	}
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glUseProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	GLfloat verts[8] = {0, 0, RES_X, 0, 0, RES_Y, RES_X, RES_Y};
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLint screen_size = glGetUniformLocation(program, "screen_size");
	glUniform2f(screen_size, RES_X, RES_Y);

	GLint win_size = glGetUniformLocation(program, "win_size");
	GLint bg_color = glGetUniformLocation(program, "bg_color");

	for (;;) {
		SDL_LockMutex(mem_mutex);
		if(mem.quit) return 0;

		int win_w, win_h;
		SDL_GetWindowSize(window, &win_w, &win_h);
		glViewport(0, 0, win_w, win_h);

		glUniform2f(win_size, win_w, win_h);
		glUniform3f(bg_color, mem.bg_col.r, mem.bg_col.g, mem.bg_col.b);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		SDL_UnlockMutex(mem_mutex);
		SDL_GL_SwapWindow(window);
	}
}

static GLuint load_shader(const char *filename, GLenum kind)
{
	const char *source = file_to_string(filename);

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
