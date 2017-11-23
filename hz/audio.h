/* #include <stdint.h> */

enum {
	HZ_SINE,
	HZ_HSINE,
	HZ_DSINE,
	HZ_QSINE,
};

enum {
	HZ_VOICES = 64,
};

struct hz_voice {
	uint8_t vol;
	uint8_t wave;
	union {
		struct { uint8_t tune, note; };
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
