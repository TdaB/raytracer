#ifndef OBJECTS_H 
#define OBJECTS_H
#include <vector>

using namespace std;


class Color
{
public:
	double r, g, b;
	Color();
	Color(double r, double g, double b);
	void print();
	Color operator * (double s);
	Color operator + (const Color &c);
	bool operator == (const Color &c);
	bool operator != (const Color &c);
};

class Point
{
public:
	double x, y, z;
	Point();
	Point(double x, double y, double z);
	void print();
	double mag();
	double distance(Point p);
	Point unitize();
	double dot(Point p);
	Point cross(Point p);
	Point operator - (const Point &p);
	Point operator + (const Point &p);
	Point operator * (double s);
	bool operator == (const Point &p);
};

class Light
{
public:
	Point position;
	Color color;
	Light();
	Light(Point p, Color c);
};

class Sphere
{
public:
	Point center;
	double radius;
	Color color;
	double k_ambient, k_diffuse, k_specular, k_reflective, n_refractive;
	int n_specular;
	Sphere();
	Sphere(
		Point center, double radius, Color color,
		double k_ambient, double k_diffuse, double k_specular,
		double k_reflective, int n_specular, double n_refractive
	);
	Point unit_normal(Point p);
};

class Scene
{
public:
    int width;
    int height;
    int camera_z;
    vector<Sphere> spheres;
    vector<Light> lights;
    int bounces;
	Scene();
    Scene(int width, int height, int camera_z, vector<Sphere> spheres, vector<Light> lights, int bounces);
};

#endif
