/* #include <stdint.h> */

enum {
	HZ_SAW,   // /|/|
	HZ_TRI,   // ^v^v
	HZ_PULSE, // -_-_
	HZ_SINE,  // nunu
	HZ_HSINE, // n_n_
	HZ_DSINE, // nnnn
	HZ_QSINE, // r_r_
};

enum {
	HZ_VOICES = 4096,
};

struct hz_voice {
	uint8_t vol;
	uint8_t wave;
	uint8_t flags;
	union {
		struct { uint8_t tune, note; };
		struct { uint8_t index; };
		uint16_t pitch;
	};
};

struct hz_amem {
	uint8_t vol[HZ_VOICES];
	uint8_t wave[HZ_VOICES];
	struct hz_voice voices[HZ_VOICES];
	uint16_t tick_rate;
};

void hz_ainit(uint16_t tick_rate);
void hz_aquit(void);
void hz_aupdate(void (*fn)(struct hz_amem*, void*), void *data);
