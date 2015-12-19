#include <math.h>
#include "utils.h"
#include <iostream>

#define EPSILON .00001 // floating point error compensation

// Doesn't return true for
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


Point reflect(Point origin, Point intersection, Point normal) {
	Point I = (intersection - origin).unitize();
	double cos_i = normal.dot(I * -1.0);
	return I + normal * 2 * cos_i;
}


Point refract(Point origin, Point intersection, Point normal, double n1, double n2) {
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


double reflectance(Point origin, Point intersection, Point normal, double n1, double n2) {
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


Color trace(Point origin, Point p, Sphere * spheres, int num_spheres, Light * lights, int num_lights, int depth) {
	Color fill(0, 0, 0);
	if (depth <= 0) {
		return fill;
	}

	Point closest_p = Point(0, 0, INFINITY);
	Sphere closest_s;

	// Get closest intersection
	for (int i = 0; i < num_spheres; i++) {
		Sphere s = spheres[i];
		Point intersection = ray_sphere_intersect(origin, p, s);

		if (intersection.z > 0) {
			if (intersection.z < closest_p.z) {
				closest_p = intersection;
				closest_s = s;
			}
		}
	}

	if (closest_p.z < INFINITY) {
		// Handle clear spheres
		if (closest_s.n_refractive > 0.0) {
			Point normal = closest_s.unit_normal(closest_p);
			Point incident = (closest_p - origin).unitize();
			Point reflected;
			Point refracted;
			double refl;
			if (incident.dot(normal) > EPSILON) { // Are we inside a sphere?
				normal = normal * -1.0;
				refl = reflectance(origin, closest_p, normal, closest_s.n_refractive, 1);
				reflected = reflect(origin, closest_p, normal);
				refracted = refract(origin, closest_p, normal, closest_s.n_refractive, 1);
			}
			else {
				refl = reflectance(origin, closest_p, normal, 1, closest_s.n_refractive);
				reflected = reflect(origin, closest_p, normal);
				refracted = refract(origin, closest_p, normal, 1, closest_s.n_refractive);	
			}
			if (refl < EPSILON) { // refl == 0
				return trace(closest_p, closest_p + refracted, spheres, num_spheres, lights, num_lights, --depth);
			}
			else if (refl > 1.0 - EPSILON) { // refl == 1
				return trace(closest_p, closest_p + reflected, spheres, num_spheres, lights, num_lights, --depth);
			}
			else {
				return trace(closest_p, closest_p + reflected, spheres, num_spheres, lights, num_lights, --depth) * refl +
					trace(closest_p, closest_p + refracted, spheres, num_spheres, lights, num_lights, --depth) * (1.0 - refl);
			}	
		}

		//Handle opaque spheres
		Color ambient = closest_s.color * closest_s.k_ambient;
		Color diffuse;
		Color specular;
		Color reflective;
		Point N = closest_s.unit_normal(closest_p);
		Point V = (origin - closest_p).unitize();
		Point R = reflect(origin, closest_p, N);
		
		for (int j = 0; j < num_lights; j++) {
			Light light = lights[j];
			Point L = (light.position - closest_p).unitize();
			Point H = (V + L).unitize();
			double d_light = (light.position - closest_p).mag();	

			 //Check to see if intersection point is blocked from light
			bool is_blocked = false;
			for (int k = 0; k < num_spheres; k++) {
				Point shadow = ray_sphere_intersect(closest_p, light.position, spheres[k]);
				if (shadow.z > 0.0) {
					double d_closest_p = (closest_p - shadow).mag();
					if (d_light > d_closest_p) {
						is_blocked = true;
						break;
					}
				}
			}
			if (!is_blocked) {
				if (closest_s.k_diffuse > 0.0) {
					double n_dot_l = N.dot(L);
					if (n_dot_l > EPSILON) {
						diffuse = diffuse + closest_s.color * closest_s.k_diffuse * n_dot_l;
					}
				}
				if (closest_s.k_specular > 0.0) {
					double n_dot_h = N.dot(H);
					if (n_dot_h > EPSILON) {
						specular = specular + light.color * closest_s.k_specular * pow(n_dot_h, closest_s.n_specular);
					}		
				}
			}
		}
		if (closest_s.k_reflective > 0.0) {
			reflective = reflective + trace(closest_p, closest_p + R, spheres, num_spheres, lights, num_lights, --depth) * closest_s.k_reflective;
		}
		fill = ambient + diffuse + specular + reflective;
	}
	return fill;
}