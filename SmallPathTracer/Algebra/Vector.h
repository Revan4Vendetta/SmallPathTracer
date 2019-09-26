#pragma once
/* Vector class with all the operations needed*/

#include <math.h>

class Vector
{

	double x;
	double y;
	double z;

public:

	// Constructors

	Vector();
	Vector(double, double, double);
	~Vector();

	// Operators
	Vector operator+(const Vector &vector) const {
		return Vector(vector.x + x, vector.y + y, vector.z + z);
	}

	Vector operator-(const Vector &vector) const {
		return Vector(vector.x - x, vector.y - y, vector.z - z);
	}

	Vector operator*(const Vector &vector) const {
		return Vector(vector.x * x, vector.y * y, vector.z * z);
	}
	
	Vector operator*(const double constant) const {
		return Vector(constant * x, constant * y, constant * z);
	}

	Vector normalize() {
		*this = *this * (1 / sqrt(x*x + y*y + z*z));
	}

	double dot(const Vector &vector) const {
		return x*vector.x + y*vector.y + z*vector.z;
	}

	Vector cross(const Vector &vector) const {
		return Vector(y*vector.z - z*vector.y, z*vector.x - x*vector.z, x*vector.y - y*vector.x);
	}

};

Vector::Vector()
{
	x = 0, y = 0, z = 0;
}

Vector::Vector(double _x, double _y, double _z) {

	x = _x;
	y = _y;
	z = _z;

}

Vector::~Vector()
{
}
