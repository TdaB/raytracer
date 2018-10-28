#include <math.h>
#include <iostream>
#include <vector>
#include "objects.h"


Color::Color()
{
	this->r = this->g = this->b = 0;
}

Color::Color(double r1, double g1, double b1)
{
	this->r = r1;
	this->g = g1;
	this->b = b1;
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
	this->x = x1;
	this->y = y1;
	this->z = z1;
}

void Point::print() {
	std::cout << "( " << this->x << ", " << this->y << ", " << this->z << " )";
}

double Point::mag()
{
	return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

double Point::distance(Point p)
{
	return sqrt((this->x - p.x) * (this->x - p.x) + (this->y - p.y) * (this->y - p.y) + (this->z - p.z) * (this->z - p.z));
}

Point Point::unitize()
{
	double m = this->mag();
	return Point(this->x / m, this->y / m, this->z / m);
}

double Point::dot(Point p)
{
	return this->x*p.x + this->y*p.y + this->z*p.z;
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

Light::Light() {
}

Light::Light(Point p, Color c) 
{
	this->position = p;
	this->color = c;
}

Sphere::Sphere() {
}

Sphere::Sphere(
	Point center, double radius, Color color, 
	double k_ambient, double k_diffuse, double k_specular,
	int n_specular, double k_reflective, double n_refractive)
{
	this->center = center;
	this->radius = radius;
	this->color = color;
	this->k_ambient = k_ambient;
	this->k_diffuse = k_diffuse;
	this->k_specular = k_specular;
	this->n_specular = n_specular;
	this->k_reflective = k_reflective;
	this->n_refractive = n_refractive;
}

Point Sphere::unit_normal(Point p) {
	return Point(
		(p.x - this->center.x) / this->radius,
		(p.y - this->center.y) / this->radius,
		(p.z - this->center.z) / this->radius
	);
}

Plane::Plane() {
}

Plane::Plane(
	Point p1, Point p2, Point p3, Color color,
	double k_ambient, double k_diffuse, double k_specular,
	int n_specular, double k_reflective)
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->normal = ((p2 - p1) * -1).cross(p3 - p1).unitize();
	this->d = -this->normal.dot(p1);
	this->color = color;
	this->k_ambient = k_ambient;
	this->k_diffuse = k_diffuse;
	this->k_specular = k_specular;
	this->n_specular = n_specular;
	this->k_reflective = k_reflective;
}

Scene::Scene() {
}

Scene::Scene(int width, int height, vector<Sphere> spheres, vector<Plane> planes, vector<Light> lights, int bounces) {
    this->width = width;
    this->height = height;
    this->spheres = spheres;
	this->planes = planes;
    this->lights = lights;
    this->bounces = bounces;
}

Properties::Properties() {
}

Properties::Properties(Color color, double k_ambient, double k_diffuse, double k_specular,
	int n_specular, double k_reflective, double n_refractive) 
{
	this->color = color;
	this->k_ambient = k_ambient;
	this->k_diffuse = k_diffuse;
	this->k_specular = k_specular;
	this->n_specular = n_specular;
	this->k_reflective = k_reflective;
	this->n_refractive = n_refractive;
}
Properties::Properties(Color color, double k_ambient, double k_diffuse,
	double k_specular, int n_specular, double k_reflective)
{
	this->color = color;
	this->k_ambient = k_ambient;
	this->k_diffuse = k_diffuse;
	this->k_specular = k_specular;
	this->n_specular = n_specular;
	this->k_reflective = k_reflective;
}
