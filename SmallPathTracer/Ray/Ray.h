#pragma once
/* Ray class */

#include "../Algebra/Vector.h"

class Ray
{

	Vector origin;
	Vector direction;

public:
	Ray();
	Ray(Vector, Vector);
	~Ray();

private:

};

Ray::Ray()
{
	origin = Vector();
	direction = Vector();
}


Ray::Ray(Vector _origin, Vector _direction) {

	origin = _origin;
	direction = _direction;

}

Ray::~Ray()
{
}
