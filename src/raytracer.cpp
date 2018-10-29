#include <math.h>
#include <iostream>
#include <ctime>
#include "raytracer.h"
#include "main.h"

#define EPSILON .0000001 // floating point error compensation


Raytracer::Raytracer(Scene s) {
	this->scene = s;
}

Color Raytracer::trace(Point origin, Point p, int bounces_left) {
    if (bounces_left <= 0) {
        return Color(0, 0, 0);
    }
    Point closest_intersection = Point(0, 0, INFINITY);
	Point closest_normal;
	Properties closest_properties;
    for (Sphere s : this->scene.spheres) {
        Point intersection = ray_sphere_intersect(origin, p, s);
        if (intersection.z > 0) {
            if (intersection.z < closest_intersection.z) {
                closest_intersection = intersection;
				closest_normal = s.unit_normal(intersection);
				closest_properties = s.properties;
            }
        }
    }
	for (Plane plane : this->scene.planes) {
		Point intersection = ray_plane_intersect(origin, p, plane);
		if (intersection.z > 0) {
			if (intersection.z < closest_intersection.z) {
				closest_intersection = intersection;
				closest_normal = plane.normal;
				closest_properties = plane.properties;
			}
		}
	}
	for (Triangle t : this->scene.triangles) {
		Point intersection = ray_triangle_intersect(origin, p, t);
		if (intersection.z > 0) {
			if (intersection.z < closest_intersection.z) {
				closest_intersection = intersection;
				closest_normal = t.normal;
				closest_properties = t.properties;
			}
		}
	}
    if (closest_intersection.z == INFINITY) {
        return Color(0,0,0);
    }
	return trace_point(origin, closest_intersection, closest_normal, closest_properties, bounces_left);
}

Color Raytracer::trace_point(Point origin, Point intersection, Point normal, Properties properties, int bounces_left) {
	if (properties.n_refractive > 0.0) {
		return trace_transparent(origin, intersection, normal, properties.n_refractive, bounces_left);
	}
	Color ambient = properties.color * properties.k_ambient;
	Color diffuse;
	Color specular;
	Color reflective;
	Point N = normal;
	Point V = (origin - intersection).unitize();
	for (Light light : this->scene.lights) {
		// Check each light to see if intersection point is blocked from light
		Point L = (light.position - intersection).unitize();
		Point H = (V + L).unitize();
		if (light_blocked(light, intersection)) {
			continue;
		}
		if (properties.k_diffuse > 0.0) {
			// Get diffuse
			double n_dot_l = N.dot(L);
			if (n_dot_l > 0) {
				diffuse = diffuse + properties.color * properties.k_diffuse * n_dot_l;
			}
		}
		if (properties.k_specular > 0.0) {
			// Get specular
			double n_dot_h = N.dot(H);
			if (n_dot_h > 0) {
				specular = specular + light.color * properties.k_specular * pow(n_dot_h, properties.n_specular);
			}
		}
	}
	if (properties.k_reflective > 0.0) {
		Point R = reflect(origin, intersection, N);
		// Add N * EPSILON to origin to cure dat acne
		reflective = reflective + trace(intersection + N * EPSILON, intersection + R, --bounces_left) * properties.k_reflective;
	}
	return ambient + diffuse + specular + reflective;
}

bool Raytracer::light_blocked(Light light, Point p) {
	double d_light = (light.position - p).mag();
	for (Sphere s : this->scene.spheres) {
		if (s.properties.n_refractive > 0) {
			continue;
		}
		Point shadow = ray_sphere_intersect(p, light.position, s);
		if (shadow.z > 0.0) {
			double d_intersection = (p - shadow).mag();
			if (d_light > d_intersection) {
				return true;
			}
		}
	}
	for (Plane plane : this->scene.planes) {
		Point shadow = ray_plane_intersect(p, light.position, plane);
		if (shadow.z > 0.0) {
			double d_intersection = (p - shadow).mag();
			if (d_light > d_intersection) {
				return true;
			}
		}
	}
	for (Triangle t : this->scene.triangles) {
		Point shadow = ray_triangle_intersect(p, light.position, t);
		if (shadow.z > 0.0) {
			double d_intersection = (p - shadow).mag();
			if (d_light > d_intersection) {
				return true;
			}
		}
	}
	return false;
}

Color Raytracer::trace_transparent(Point origin, Point intersection, Point normal, double n_refractive, int bounces_left) {
	//Point normal = s.unit_normal(intersection);
	Point incident = (intersection - origin).unitize();
	Point reflected;
	Point refracted;
	double refl;
	if (incident.dot(normal) > EPSILON) { // Are we inside a sphere?
		normal = normal * -1.0;
		refl = reflectance(origin, intersection, normal, n_refractive, 1);
		reflected = reflect(origin, intersection, normal);
		refracted = refract(origin, intersection, normal, n_refractive, 1);
	}
	else {
		refl = reflectance(origin, intersection, normal, 1, n_refractive);
		reflected = reflect(origin, intersection, normal);
		refracted = refract(origin, intersection, normal, 1, n_refractive);
	}
	if (refl < EPSILON) { // refl == 0
		return trace(intersection, intersection + refracted, --bounces_left);
	}
	else if (refl > 1.0 - EPSILON) { // refl == 1
		return trace(intersection, intersection + reflected, --bounces_left);
	}
	else {
		return trace(intersection, intersection + reflected, --bounces_left) * refl +
			trace(intersection, intersection + refracted, --bounces_left) * (1.0 - refl);
	}
}

Point Raytracer::ray_sphere_intersect(Point p0, Point p1, Sphere s) {
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
	double t;

	if (disc < 0.0) {
		return Point(0, 0, -69);
	}
	t = (-b - sqrt(disc)) / (2.0 * a);
	if (t < EPSILON) {
		t = (-b + sqrt(disc)) / (2.0 * a);
	}
	if (t < EPSILON) {
		return Point(0, 0, -69);
	}
	return Point(p0.x + t * dx, p0.y + t * dy, p0.z + t * dz);
}

Point Raytracer::ray_plane_intersect(Point p0, Point p1, Plane plane) {
	Point direction = (p1 - p0).unitize();
	double denom = plane.normal.dot(direction);
	if (denom <= 0  && denom > -EPSILON) {
		return Point(0, 0, -69);
	}
	double num = -(plane.normal.dot(p0) + plane.d);
	double t = num / denom;
	if (t < EPSILON) {
		return Point(0, 0, -69);
	}
	return p0 + direction * t;
}

Point Raytracer::ray_triangle_intersect(Point p0, Point p1, Triangle triangle) {
	Point direction = (p1 - p0).unitize();
	double denom = triangle.normal.dot(direction);
	if (denom <= 0 && denom > -EPSILON) {
		return Point(0, 0, -69);
	}
	double num = -(triangle.normal.dot(p0) + triangle.d);
	double t = num / denom;
	if (t < EPSILON) {
		return Point(0, 0, -69);
	}
	Point intersection = p0 + direction * t;
	if (triangle.contains_point(intersection)) {
		return intersection;
	}
	return Point(0, 0, -69);
}

Point Raytracer::reflect(Point origin, Point intersection, Point normal) {
	Point I = (intersection - origin).unitize();
	double cos_i = normal.dot(I * -1.0);
	return I + normal * 2 * cos_i;
}

Point Raytracer::refract(Point origin, Point intersection, Point normal, double n1, double n2) {
	Point I = (intersection - origin).unitize();
	double n = n1 / n2;
	double cos_i = normal.dot(I * -1.0);
	double sin_t_2 = n * n * (1.0 - cos_i * cos_i);
	if (sin_t_2 > 1.0 - EPSILON) {
		return Point(0, 0, -69);
	}
	double cos_t = sqrt(1.0 - sin_t_2);
	return I * n + normal * (n * cos_i - cos_t);
}

double Raytracer::reflectance(Point origin, Point intersection, Point normal, double n1, double n2) {
	Point I = (intersection - origin).unitize();
	double n = n1 / n2;
	double cos_i = normal.dot(I * -1.0);
	double sin_t_2 = n * n * (1.0 - cos_i * cos_i);
	if (sin_t_2 > 1.0 - EPSILON) {
		return 1.0;
	}
	double cos_t = sqrt(1.0 - sin_t_2);
	double R_perp = (n1 * cos_i - n2 * cos_t) / (n1 * cos_i + n2 * cos_t);
	double R_para = (n2 * cos_i - n1 * cos_t) / (n2 * cos_i + n1 * cos_t);
	return (R_perp * R_perp + R_para * R_para) / 2.0;
}
