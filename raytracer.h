#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "objects.h"
#include <vector>


class Raytracer
{
public:
	Scene scene;
    vector<Sphere> spheres;
    vector<Light> lights;
    Raytracer(Scene s);
    Point ray_sphere_intersect(Point p0, Point p1, Sphere s);
    Point reflect(Point origin, Point intersection, Point normal);
    Point refract(Point origin, Point intersection, Point normal, double n1, double n2);
    double reflectance(Point origin, Point intersection, Point normal, double n1, double n2);
    Color trace_transparent(Point origin, Point intersection, Sphere s, int bounces_left);
    Color trace(Point origin, Point p, int bounces_left);
};

#endif
