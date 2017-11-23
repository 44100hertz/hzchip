/* #include <stdint.h> */

enum {
	HZ_SINE,
	HZ_HSINE,
	HZ_DSINE,
	HZ_QSINE,
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
	struct hz_voice voices;
	uint16_t tick_rate;
};

void hz_ainit(void);
void hz_aquit(void);
void hz_aupdate(void (*fn)(struct hz_amem*, void*), void *data);
