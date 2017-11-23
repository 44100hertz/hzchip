#include <SDL2/SDL.h>
#include <assert.h>
#include <math.h>

#include "audio.h"

static int16_t get_wave(char wave, uint16_t index);

static SDL_mutex *lock;
static int srate;
static struct hz_amem static_mem;

enum {
	SINE_BITS = 8,
	WAVE_BITS = SINE_BITS + 2,
	SINE_SIZE = (1 << SINE_BITS),
	WAVE_SIZE = (1 << WAVE_BITS),
};

struct {
	uint16_t sample_count;
	uint16_t next_tick;
	struct imem_voice {
		uint16_t phase;
		uint16_t phase_inc;
	} voices[HZ_VOICES];
	// The first 1/4 of a sine wave, later reconstructed into full wave.
	uint8_t sine[SINE_SIZE];
} imem;

static void callback(void *data, Uint8 *raw_stream, int len);

void hz_ainit(uint16_t tick_rate)
{
	SDL_Init(SDL_INIT_AUDIO);

	lock = SDL_CreateMutex();

	SDL_AudioSpec want;
	memset(&want, 0, sizeof(want));
	want.freq = 48000;
	want.format = AUDIO_S16;
	want.channels = 2;
	want.callback = callback;
	SDL_AudioSpec have;
	const int dev = SDL_OpenAudioDevice(
		NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE
	);

	srate = have.freq;
	memset(&imem, 0, sizeof(imem));
	memset(&static_mem, 0, sizeof(static_mem));
	static_mem.tick_rate = tick_rate;

	const double pi = acos(-1);
	for(int i=0; i<SINE_SIZE; ++i) {
		double thru = (double)i / SINE_SIZE;
		double circ_pos = thru * pi / 2;
		imem.sine[i] = UINT8_MAX * sin(circ_pos);
	}

	SDL_PauseAudioDevice(dev, 0);
}

void hz_aquit(void)
{
}

void hz_aupdate(void (*fn)(struct hz_amem*, void*), void *data)
{
	SDL_LockMutex(lock);
	fn(&static_mem, data);
	SDL_UnlockMutex(lock);
}

static void tick(struct hz_amem *mem)
{
	assert(mem->tick_rate);
	imem.next_tick += srate / mem->tick_rate;
	for(int i=0; i<HZ_VOICES; ++i) {
		struct imem_voice *const iv = imem.voices+i;
		struct hz_voice *const v = mem->voices+i;
		const double pitch = (double)v->pitch / 256.0;
		const double offset = (pitch - 60.0) / 12.0;
		iv->phase_inc = (1<<15) * 440.0 * pow(2.0, offset) / srate;
	}
}

static void callback(void *data, Uint8 *raw_stream, int len)
{
	SDL_LockMutex(lock);
	struct hz_amem mem = static_mem;
	SDL_UnlockMutex(lock);

	int16_t *const stream = (int16_t*)raw_stream;

	for(int i=0; i<len/2; ++i) {
		imem.sample_count++;
		if (imem.sample_count == imem.next_tick) {
			SDL_LockMutex(lock);
			mem = static_mem;
			tick(&mem);
			SDL_UnlockMutex(lock);
		}
		int16_t total = 0;
		for(int i=0; i<HZ_VOICES; ++i) {
			struct imem_voice *const iv = imem.voices+i;
			struct hz_voice *const v = mem.voices+i;
			total += get_wave(v->wave, iv->phase) * v->vol;
			iv->phase += iv->phase_inc;
		}
		stream[i] = total;
	}
}

static int16_t get_wave(char wave_type, uint16_t index) {
	index >>= 16 - WAVE_BITS;
	const int16_t flipx = index & (1 << (WAVE_BITS - 2));
	const int16_t flipy = index & (1 << (WAVE_BITS - 1));
	const uint8_t pos = flipx ? SINE_SIZE - index - 1 : index;
	const uint8_t wave = imem.sine[pos];
	switch (wave_type) {
	case HZ_SINE:
		return flipy ? -wave : wave;
	case HZ_HSINE:
		return flipy ? 0 : wave;
	case HZ_DSINE:
		return wave;
	case HZ_QSINE:
		return flipx || flipy ? 0 : wave;
	default:
		return 0;
	}
}
