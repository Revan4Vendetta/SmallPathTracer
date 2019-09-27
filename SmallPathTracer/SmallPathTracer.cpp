#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "Ray\Ray.h"
#include "Engine\PathTracer.h"
#include "RandomNumbers.h"

using namespace std;

/**
 * Main class where we launch the engine to render a demo image.
 * Right now it is just a pink sphere.
 */
int main()
{

	int width = 1024;
	int heigh = 768;
	int samps = 16;

	Ray camera = Ray(Vector(50, 0, 0), Vector(-1, 0, 0).normalize()); // Camera position and direction

	Vector cx = Vector(width*.5135 / heigh);
	Vector cy = Vector(cx.cross(camera.direction).normalize() * .5135);
	Vector radiance;

	Vector *c = new Vector[width*heigh];

	unsigned int Xi = 0; // Random seed!
	srand(0);

	// Set the world!
	vector<Sphere> world;

	// Light
	Sphere light = Sphere(600, Vector(0, 10, 0), Vector(12, 12, 12), Vector(), DIFF);

	// Walls
	Sphere left = Sphere(10, Vector(0, 0, 0), Vector(), Vector(.75, .25, .25), DIFF);

	world.push_back(light);
	world.push_back(left);

	Engine engine = Engine(world);

	for (int y = 0; y < heigh; y++) // loop over image rows
	{
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100.*y / (heigh / 1));
		for (int x = 0; x < width; x++) // Loop columns
		{
			for (int sy = 0, i = (heigh - y - 1)*width + x; sy < 2; sy++) // 2x2 subpixels rows
			{
				for (int sx = 0; sx < 2; sx++, radiance = Vector()) // 2x2 subpixels columns
				{
					for (int s = 0; s < samps; s++)
					{

						double r1 = rand() / static_cast<double>(RAND_MAX);
						r1 *= 2;

						double dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);

						double r2 = 2 * (rand() / static_cast<double>(RAND_MAX));
						double dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);

						Vector d = cx*(((sx + .5 + dx) / 2 + x) / width - .5) +
							cy*(((sy + .5 + dy) / 2 + y) / heigh - .5) + camera.direction;

						radiance = radiance +
							engine.calculateRadiance(Ray(camera.origin + d * 140, d.normalize()), 0, &Xi)*(1. / samps);
					}
					c[i] = c[i] + Vector(clamp(radiance.x), clamp(radiance.y), clamp(radiance.z)) * .25;
				}
			}
		}
	}

	// Writing the image 
	FILE *file = fopen("david_image.ppm", "w");
	fprintf(file, "P3\n%d %d\n%d ", width, heigh, 255);
	for (int i = 0; i < width*heigh; i++)
		fprintf(file, "%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));

	fflush(file);
	fclose(file);

}