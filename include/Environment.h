#pragma once

#include "Camera.h"
#include <vector>

class Environment {
public:
    Environment(int width, int height);
    void render(std::vector<uint32_t> &framebuffer);
private:
	Camera camera;
	int width;
	int height;
};