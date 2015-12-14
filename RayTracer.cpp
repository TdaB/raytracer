#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <algorithm>
#include "objects.h"
#include "utils.h"

#define WIDTH 200
#define HEIGHT 600

using namespace std;


void render_pixel(int x, int y, Color c) {
	glBegin(GL_POINTS);
	glColor3d(c.r, c.g, c.b);
	glVertex2i(x, y);
	glEnd();
}

Color trace(Point origin, Point p, Sphere * spheres, Light * lights) {
	Color fill(0, 0, 0);
	for (int i = 0; i < 2; i++) {
		Sphere s = spheres[i];
		Point intersection = ray_sphere_intersect(origin, p, s);

		if (intersection.z > 0) {
			Color ambient = s.color * s.k_ambient;
			Color diffuse;
			Color specular;
			Point N = s.unit_normal(intersection);
			Point V = (origin - intersection).unitize();
			for (int j = 0; j < 2; j++) {
				Light light = lights[j];
				Point L = (light.position - intersection).unitize();
				Point H = (V + L).unitize();
				Point R = N * 2 * N.dot(L) - L;

				// Check to see if intersection point is blocked from light
				bool blocked = false;
				for (int k = 0; k < 2; k++) {
					Point shadow = ray_sphere_intersect(intersection, light.position, spheres[k]);
					if (shadow.z > 0) {
						blocked = true;
						break;
					}
				}
				if (!blocked) {
					diffuse = diffuse + s.color * s.k_diffuse * N.dot(L);
					specular = specular + light.color * s.k_specular * pow(N.dot(H), s.n_specular);
				}						
			}
			fill = ambient + diffuse + specular;
		}
	}
	return fill;

}


void render_scene()
{
	Point camera = Point(0, 0, -800);

	Light lights[] = {
		Light(Point(50, 1000, 0), Color(1, 1, 1)),
		Light(Point(0, 1000, 200), Color(1, 1, 1))
	};

	Sphere spheres[] = {
		Sphere(Point(0, 100, 200), 70, Color(1, 0, 0), .05, .5, .5, 40),
		Sphere(Point(0, -100, 200), 100, Color(0, 1, 1), .05, .5, .5, 12)
	};
	
	for (int x = -WIDTH / 2; x < WIDTH / 2; x++) {
		for (int y = -HEIGHT / 2; y < HEIGHT / 2; y++) {
			Point pixel = Point(x, y, 0);
			Color fill = trace(camera, pixel, spheres, lights);
			render_pixel(x, y, fill);
		}
	}
	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("OpenGL First Window");

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);
	//define coords in viewing area
	gluOrtho2D(-WIDTH / 2.0, WIDTH / 2.0, -HEIGHT / 2.0, HEIGHT / 2.0);
	// 2 days of my life gone from this damn option
	glViewport(0, 0, WIDTH, HEIGHT);

	// register callbacks
	glutDisplayFunc(render_scene);
	glutMainLoop();

	return 0;
}