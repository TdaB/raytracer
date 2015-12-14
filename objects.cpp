#include <math.h>
#include <iostream>
#include "objects.h"


Color::Color()
{
	this->r = this->g = this->b = 0;
}

Color::Color(double r1, double g1, double b1)
{
	r = r1;
	g = g1;
	b = b1;
}

void Color::print() {
	std::cout << "( " << this->r << ", " << this->g << ", " << this->b << " )";
}

Color Color::operator + (const Color &c)
{
	return Color(this->r + c.r, this->g + c.g, this->b + c.b);
}

Color Color::operator * (double s) {
	return Color(this->r * s, this->g * s, this->b * s);
}

bool Color::operator == (const Color &c) {
	return this->r == c.r && this->g == c.g && this->b == c.b;
}

bool Color::operator != (const Color &c) {
	return !(this->r == c.r && this->g == c.g && this->b == c.b);
}



Point::Point() 
{
	this->x = this->y = this->z = 0;
}

Point::Point(double x1, double y1, double z1)
{
	x = x1;
	y = y1;
	z = z1;
}

void Point::print() {
	std::cout << "( " << this->x << ", " << this->y << ", " << this->z << " )";
}

double Point::mag()
{
	return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

Point Point::unitize()
{
	double m = this->mag();
	return Point(this->x / m, this->y / m, this->z / m);
}

double Point::dot(Point p)
{
	double answer = this->x*p.x + this->y*p.y + this->z*p.z;
	if (answer < 0) return 0;
	return answer;
}

Point Point::cross(Point p)
{
	return Point(
		this->y*p.z - this->z*p.y,
		this->z*p.x - this->x*p.z,
		this->x*p.y - this->y*p.x
	);
}

Point Point::operator + (const Point &p)
{
	return Point(this->x + p.x, this->y + p.y, this->z + p.z);
}

Point Point::operator - (const Point &p)
{
	return Point(this->x - p.x, this->y - p.y, this->z - p.z);
}

Point Point::operator * (double s)
{
	return Point(this->x * s, this->y * s, this->z * s);
}

bool Point::operator == (const Point &p)
{
	return this->x == p.x && this->y == p.y && this->z == p.z;
}


Light::Light() 
{
	this->position = Point();
	this->color = Color();
}

Light::Light(Point p, Color c) 
{
	this->position = p;
	this->color = c;
}


Sphere::Sphere()
{
	this->center = Point();
	this->radius = 0;
	this->color = Color();
	this->k_ambient = this->k_diffuse = this->k_specular = this->n_specular = 0;
}

Sphere::Sphere(
	Point my_center, double my_radius, Color my_color, 
	double k_ambient, double k_diffuse, double k_specular, int n_specular)
{
	this->center = my_center;
	this->radius = my_radius;
	this->color = my_color;
	this->k_ambient = k_ambient;
	this->k_diffuse = k_diffuse;
	this->k_specular = k_specular;
	this->n_specular = n_specular;
}

Point Sphere::unit_normal(Point p) {
	return Point(
		(p.x - this->center.x) / this->radius,
		(p.y - this->center.y) / this->radius,
		(p.z - this->center.z) / this->radius
	);
}