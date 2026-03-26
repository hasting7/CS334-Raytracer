#include <vector>

#include "Environment.h"
#include "Camera.h"

Environment::Environment(int width, int height) : width(width), height(height) {
	this->camera = Camera();
}

void Environment::render(std::vector<uint32_t> &framebuffer) {
	int x_rel, y_rel;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			x_rel = 255 * ((float)i / height);
			y_rel = 255 * ((float)j / width);
			framebuffer[i * width + j] = 0xFF000000 | ((x_rel  & 0xFF) << 16) | ((y_rel  & 0xFF) << 8);
		}
	}

}