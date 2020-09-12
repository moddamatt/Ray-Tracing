#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

/*
* Steps for ray tracing
* (1) calculate the ray from the eye to the pixel
* (2) determine which objects the ray intersects
* (3) compute a color for that intersection point
*/

// Override base class with your custom functionality
class RayTracer : public olc::PixelGameEngine
{
public:
	RayTracer()
	{
		// Name you application
		sAppName = "Ray Tracing in a Weekend";
	}

public:
	bool OnUserCreate() override
	{
		const auto aspect_ratio = 3.0 / 2.0;
		const int samples_per_pixel = 50;
		const int max_depth = 10;

		// World
		auto world = random_scene();

		// Camera
		point3 lookfrom(13, 2, 3);
		point3 lookat(0, 0, 0);
		vec3 vup(0, 1, 0);
		auto dist_to_focus = 10.0;
		auto aperture = 0.1;

		camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

		// Called once at the start, so create things here
		for (int y = ScreenHeight() - 1; y >= 0; --y) {
			std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
			for (int x = 0; x < ScreenWidth(); ++x)
			{
				color pixel_color(0, 0, 0);
				for (int s = 0; s < samples_per_pixel; ++s) {
					auto u = (x + random_double()) / (ScreenWidth() - 1);
					auto v = (y + random_double()) / (ScreenHeight() - 1);
					ray r = cam.get_ray(u, v);
					pixel_color += ray_color(r, world, max_depth);
				}
				render(x, y, pixel_color, samples_per_pixel); 
			}
			
		}

		std::cerr << "\nDone.\n";
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame, draws random coloured pixels
		return true;
	}

	void render(int x, int y, color pixel_color, int samples_per_pixel)
	{
		auto r = pixel_color.x();
		auto g = pixel_color.y();
		auto b = pixel_color.z();

		// Divide the color by the number of samples and gamma-correct for gamma=2.0.
		auto scale = 1.0 / samples_per_pixel;
		r = sqrt(scale * r);
		g = sqrt(scale * g);
		b = sqrt(scale * b);

		r = static_cast<int>(256 * clamp(r, 0.0, 0.999));
		g = static_cast<int>(256 * clamp(g, 0.0, 0.999));
		b = static_cast<int>(256 * clamp(b, 0.0, 0.999));
		Draw(x, y, olc::Pixel(r, g, b));
	}

	double hit_sphere(const point3& center, double radius, const ray& r) {
		vec3 oc = r.origin() - center;
		auto a = r.direction().length_squared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.length_squared() - radius * radius;
		auto discriminant = half_b * half_b - a * c;

		if (discriminant < 0) {
			return -1.0;
		}
		else {
			return (-half_b - sqrt(discriminant)) / a;
		}
	}

	color ray_color(const ray& r, const hittable& world, int depth) {
		hit_record rec;

		// If we've exceeded the ray bounce limit, no more light is gathered.
		if (depth <= 0)
			return color(0, 0, 0);

		if (world.hit(r, 0.001, infinity, rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, world, depth - 1);
			return color(0, 0, 0);
		}
		vec3 unit_direction = unit_vector(r.direction());
		auto t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
	}

	hittable_list random_scene() {
		hittable_list world;

		auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
		world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

		for (int a = -11; a < 11; a++) {
			for (int b = -11; b < 11; b++) {
				auto choose_mat = random_double();
				point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

				if ((center - point3(4, 0.2, 0)).length() > 0.9) {
					shared_ptr<material> sphere_material;

					if (choose_mat < 0.8) {
						// diffuse
						auto albedo = color::random() * color::random();
						sphere_material = make_shared<lambertian>(albedo);
						world.add(make_shared<sphere>(center, 0.2, sphere_material));
					}
					else if (choose_mat < 0.95) {
						// metal
						auto albedo = color::random(0.5, 1);
						auto fuzz = random_double(0, 0.5);
						sphere_material = make_shared<metal>(albedo, fuzz);
						world.add(make_shared<sphere>(center, 0.2, sphere_material));
					}
					else {
						// glass
						sphere_material = make_shared<dielectric>(1.5);
						world.add(make_shared<sphere>(center, 0.2, sphere_material));
					}
				}
			}
		}

		auto material1 = make_shared<dielectric>(1.5);
		world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

		auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
		world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

		auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
		world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

		return world;
	}
};

int main()
{
	// Iamge
	const auto aspect_ratio = 3.0 / 2.0;
	const int image_width = 1200;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	RayTracer demo;
	if (demo.Construct(image_width, image_height, 1, 1))
		demo.Start();
	return 0;
}