#include <random.h>
#include <assert_test.h>

uint8_t* random_byte_ptr = 0;

void init_random(uint8_t* byte_pointer) {
	random_byte_ptr = byte_pointer;
}

uint8_t rand(void) {
	return *random_byte_ptr;
}

uint8_t rand_in_range(uint8_t min, uint8_t max) {
	ASSERT(min <= max);
	ASSERT(0); /* No implementado. */

	if (min == max)
		return min;

	return max;
}
