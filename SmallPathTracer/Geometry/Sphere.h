#pragma once

/* Class to model spheres */

#include "../Algebra/Vector.h"
#include "../Material/Reflection.h"
#include "../Ray/Ray.h"

class Sphere
{
	double radius;
	Vector position;

	Vector color; // We coded color as RGB so we don't need to create another class
	Vector emission; // Do our sphere emit light? If we coded it as the color, we can do that it emits light in an specific color 

	Reflection reflection; // How this sphere reacts with light

public:

	Sphere(double, Vector, Vector, Vector, Reflection);
	~Sphere();

	Vector getPosition() { return position; }
	double getRadius() { return radius; }

	Vector getColor() { return color; }
	Vector getEmission() { return emission; }

	Reflection getReflection() { return reflection; }



	double intersect(const Ray & ray) const;

};

Sphere::Sphere(double _radius, Vector _position, Vector _color, Vector _emission, Reflection _reflection)
{
	radius = _radius;
	position = _position;
	color = _color;
	emission = _emission;
	reflection = _reflection;
}

Sphere::~Sphere()
{
}

// Intersection. How we want to calculate intersection between a ray and a sphere
// Returns 0 it the ray don't hit the sphere
// Returns distance otherwise
double Sphere::intersect(const Ray &ray) const {

	double epsilon = 1e-4; 

	Vector rayOrigin = ray.getOrigin;
	Vector op = position - rayOrigin;

	double t;
	double b = op.dot(ray.getDirection);
	double det = b*b - op.dot(op) + radius*radius;

	if (det < 0 )
	{
		return 0; // There is no interaction between ray and sphere
	}
	
	// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	return (t = b - det) > epsilon ? t : ((t = b + det) > epsilon ? t : 0);


}
