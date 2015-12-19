#ifndef UTILS_H 
#define UTILS_H
#include "objects.h"

Point ray_sphere_intersect(Point p0, Point p1, Sphere s);

Point reflect(Point origin, Point intersection, Point normal);

Point refract(Point origin, Point intersection, Point normal, double n1, double n2);

double reflectance(Point origin, Point intersection, Point normal, double n1, double n2);

Color handle_clear(Point origin, Point intersection, Sphere s);

Color trace(Point origin, Point p, Sphere * spheres, int num_spheres, Light * lights, int num_lights, int depth);

#endif