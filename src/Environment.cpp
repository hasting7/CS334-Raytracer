#include <vector>

#include "Environment.h"
#include "Camera.h"
#include "Color.h"
#include "Vec3.h"
#include "Ray.h"


Environment::Environment(int width, int height) : width(width), height(height) {
	camera = Camera();
	ray_depth = 1;
	rays_per_pixel =1 ;
}

void Environment::add_object(std::unique_ptr<Object> object) {
	objects.push_back(std::move(object));
}

void Environment::render(std::vector<uint32_t> &framebuffer) {
	Color color;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// populate with proper ray info
			// we really only need the point on the viewing plane
			// 		from there we subtract camera position from pos on viewing plane to get dir
			color = shoot_ray(Ray(camera.position, Vec3(x,y,0)), ray_depth);

			// put color to frame
			framebuffer[y * width + x] = color.to_int();
		}
	}

}

/*
returns color of what pixel value should be
*/
Color Environment::shoot_ray(const Ray& ray, int depth) {
	/*
		loop for every object in scene
			collect Hit structs
			determine closest
			use that as hit location
		recurse or base case
			use material + normal to determine ray for recursive call
	*/
	return Color(ray.dir.x,ray.dir.y,255);
}