#include "Color.h"
#include <cstdint>

uint32_t Color::to_int() const{
	return 0xFF000000 | ((r & 0xFF) << 16) | ((b & 0xFF) << 8) | (g & 0xFF);
}