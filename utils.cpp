#include <math.h>
#include "utils.h"

Point ray_sphere_intersect(Point p0, Point p1, Sphere s) {
	Point center = s.center;

	double dx = p1.x - p0.x;
	double dy = p1.y - p0.y;
	double dz = p1.z - p0.z;

	double a = dx * dx + dy * dy + dz * dz;
	double b =
		2 * dx * (p0.x - center.x) +
		2 * dy * (p0.y - center.y) +
		2 * dz * (p0.z - center.z);
	double c =
		center.x * center.x +
		center.y * center.y +
		center.z * center.z +
		p0.x * p0.x + p0.y * p0.y + p0.z * p0.z +
		-2 * (center.x * p0.x + center.y * p0.y + center.z * p0.z) -
		s.radius * s.radius;

	double disc = b * b - 4 * a * c;
	if (disc <= 0) {
		return Point(0, 0, -69);
	}
	double t = (-b - sqrt(disc)) / (2 * a);
	if (t < 0) {
		return Point(0, 0, -69);
	}
	return Point(p0.x + t * dx, p0.y + t * dy, p0.z + t * dz);
}

