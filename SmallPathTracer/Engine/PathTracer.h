#pragma once
#include "../Algebra/Vector.h"
#include "../Ray/Ray.h"
#include "../Geometry/Sphere.h"
#include "../Material/Reflection.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

#define _USE_MATH_DEFINES 
#include <math.h>
#include <cmath>

using namespace std;

/*Path tracer engine and some usefull tools*/

class Engine
{

	vector<Sphere> world;

public:
	Engine();
	Engine(vector<Sphere>);
	~Engine();

	bool intersect(Ray &ray, double &distance, int &id) const;
	Vector calculateRadiance(Ray &ray, int depth, unsigned int *Xi);

private:

};

Engine::Engine()
{
}

inline Engine::Engine(vector<Sphere> _world)
{
	world = _world;
}

Engine::~Engine()
{
}

/* We check if a ray intersect with any body in the world */
inline bool Engine::intersect(Ray & ray, double & distance, int & id) const
{
	double inf = distance = 1e20;
	double t = inf;
	double sizeOfWorld = world.size();

	int position = 0;

	for (const Sphere & sphere : world) {

		if ((distance = sphere.intersect(ray)) && distance < t) {
			t = distance;
			id = position;
		}

		position++;
	}

	return t < inf;
}


inline double clamp(double x) {
	return (x < 0 ? 0 : x>1 ? 1 : x);
}

inline int toInt(double x) {
	return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}


/**
 * Main function to calculate the radiance of an interaction depending if the object is diffuse, specular or refract the ray.
 * If the ray don't interact, It returns zero, which is used to paint it as black.
 */
Vector Engine::calculateRadiance(Ray &ray, int depth, unsigned int *Xi) {

	double distance; // distance of the intersection
	int id = 0; // ID of the intersection

	if (!intersect(ray, distance, id)) {
		// If we miss, return black
		return Vector();
	}

	const Sphere &objectHitted = world.at(id);

	Vector x = ray.origin + ray.direction*distance;
	Vector normal = (x - objectHitted.position);
	normal = normal.normalize();

	Vector nl = normal.dot(ray.direction) < 0 ? normal : normal*(-1);
	Vector color = objectHitted.color;

	// we obtain max reflection
	double maxReflection =
		color.x > color.y && color.x > color.y ? color.x : color.y > color.z ? color.y : color.z;

	if (++depth > 5) {

		double randomNumer = (rand() / static_cast<double>(RAND_MAX)) * 255;

		if (randomNumer < maxReflection)
			color = color * (1 / maxReflection);
		else
			return objectHitted.emission;
	}

	if (objectHitted.reflection == DIFF) {
		// Ideal diffuse reflecion
		double r1 = 2 * M_PI * (rand() / static_cast<double>(RAND_MAX));
		double r2 = (rand() / static_cast<double>(RAND_MAX));
		double r2sqrt = sqrt(r2);

		Vector w = nl;
		Vector u = ((fabs(w.x) > .1 ? Vector(0, 1, 0) : Vector(1, 1, 1)).cross(w)).normalize();
		Vector v = w.cross(u);

		Vector reflectionDirection = (u*cos(r1) * r2sqrt + v*sin(r1)*r2sqrt + w*sqrt(1 - r2)).normalize();

		return objectHitted.emission + color * calculateRadiance(Ray(x, reflectionDirection), depth, Xi);
		
	}
	else if (objectHitted.reflection == SPEC) {
		// Ideal specular reflection
		return objectHitted.emission + color * calculateRadiance(Ray(x, ray.direction - normal * 2 * normal.dot(ray.direction)), depth, Xi);
	}
	else {
		// Ideal dielectric refraction
		Ray reflectedRay(x, ray.direction - normal * 2 * normal.dot(ray.direction));
		bool into = normal.dot(nl) > 0; // We check if the ray come from outside

		double nc = 1;
		double nt = 1.5;
		double nnt = into ? nc / nt : nt / nc;
		double ddn = ray.direction.dot(nl);
		double cos2t;

		// Total internal reflection 
		if ((cos2t = 1 - nnt*nnt*(1 - ddn*ddn)) < 0) {

			return objectHitted.emission +
				color* calculateRadiance(reflectedRay, depth, Xi);
		}

		Vector reflectionDirection = (ray.direction*nnt - normal*((into ? 1 : -1)*(ddn*nnt + sqrt(cos2t)))).normalize();
		double a = nt - nc;
		double b = nt + nc;
		double R0 = a*a / (b*b);
		double c = 1 - (into ? -ddn : reflectionDirection.dot(normal));

		double Re = R0 + (1 - R0)*pow(c, 5);
		double Tr = 1 - Re;
		double P = .25 + .5*Re;
		double RP = Re / P;
		double TP = Tr / (1 - P);

		// Russian roulette!!!
		return objectHitted.emission + color * (depth > 2 ? ((rand() / static_cast<double>(RAND_MAX)) < P ?
			calculateRadiance(reflectedRay, depth, Xi)*RP : calculateRadiance(Ray(x, reflectionDirection), depth, Xi)*TP) :
			calculateRadiance(reflectedRay, depth, Xi)*Re + calculateRadiance(Ray(x, reflectionDirection), depth, Xi)*Tr);

	}


}


