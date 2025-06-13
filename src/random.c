#include <random.h>

uint8_t random_number = 0;

void init_random(uint16_t seed) {
	random_number = seed;
}

void next_rand(void) {
	random_number = (random_number >> 4) + 89;

	random_number ^= (random_number << 3);
	random_number ^= (random_number >> 5);
	random_number ^= (random_number << 1);
}

uint16_t get_rand(void) {
	return random_number;
}
