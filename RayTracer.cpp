#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <limits>
#include <ctime>
#include "objects.h"
#include "utils.h"

#define WIDTH 800
#define HEIGHT 600

using namespace std;


void render_pixel(int x, int y, Color c)
{
	glBegin(GL_POINTS);
	glColor3d(c.r, c.g, c.b);
	glVertex2i(x, y);
	glEnd();
	//glFlush();
}

void draw_line(Point p1, Point p2) {
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2i(p1.x, p1.y);
	glVertex2i(p2.x, p2.y);
	glEnd();
}

void render_scene(Point camera, Sphere * spheres, int num_spheres, Light * lights, int num_lights) {
	clock_t begin = clock();

	// Main loop
	for (int x = -WIDTH / 2; x < WIDTH / 2; x++) {
		for (int y = -HEIGHT / 2; y < HEIGHT / 2; y++) {
			Point pixel = Point(x, y, 0);
			Color fill = trace(camera, pixel, spheres, num_spheres, lights, num_lights, 15);
			render_pixel(x, y, fill);
		}
	}
	glFlush();

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "RENDERING TIME: " << elapsed_secs << " SECONDS";
}


//void scene1()
//{
//	int num_lights = 3;
//	int num_spheres = 4;
//	Point camera = Point(0, 0, -800);
//
//	Light lights[] = {
//		Light(Point(0, 1000, 400), Color(1, 1, 1)),
//		Light(Point(-300, 1000, 300), Color(1, 1, 1)),
//		Light(Point(0, 0, 150), Color(1, 1, 1))
//	};
//
//	Sphere spheres[] = {
//		Sphere(Point(-200, -150, 100), 80, Color(1, 0, 0), .05, .5, .5, .9, 40, 0),
//		Sphere(Point(200, -150, 100), 80, Color(.2, .7, .4), .05, .5, .5, .9, 12, 0),
//		Sphere(Point(-200, 300, 300), 80, Color(.2, .4, .5), .05, .5, .5, .9, 12, 0),
//		Sphere(Point(0, 0, 400), 200, Color(1, 1, 0), .05, .4, .1, .3, 40, 0)
//	};
//	render_scene(camera, spheres, num_spheres, lights, num_lights);
//}

void scene2()
{
	int num_lights = 3;
	int num_spheres = 9;
	Point camera = Point(0, 0, -800);

	Light lights[] = {
		Light(Point(0, 0, 0), Color(1, 1, 1)),
		Light(Point(-300, 0, 500), Color(1, 1, 1)),
		Light(Point(500, 0, 420), Color(1, 1, 1))
	};

	Sphere spheres[] = {
		Sphere(Point(-100, -100, 220), 100, Color(1, 0, 0), .05, .5, .5, .9, 40, 3),
		Sphere(Point(100, -100, 220), 100, Color(1, 0, 0), .05, .5, .5, .9, 40, 2),
		Sphere(Point(-100, 100, 220), 100, Color(1, 0, 0), .05, .5, .5, .9, 40, 1.1),
		Sphere(Point(100, 100, 320), 100, Color(1, 0, 0), .05, .5, .5, .9, 40, 1.5),
		Sphere(Point(150, 0, 430), 100, Color(1, 0, 0), .05, .5, .5, .9, 40, 1.5),

		Sphere(Point(200, -150, 350), 80, Color(.2, .7, .4), .05, .5, .5, .9, 12, 0),
		Sphere(Point(-110, 130, 250), 80, Color(.2, .4, .5), .05, .5, .5, .9, 12, 0),
		Sphere(Point(-100, 0, 430), 100, Color(1, 0, 0), .05, .5, .5, .7, 50, 0),
		Sphere(Point(0, 100, 1600), 1000, Color(1, 1, 0), .05, .4, .1, 0, 40, 0)
	};
	render_scene(camera, spheres, num_spheres, lights, num_lights);
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
	glutDisplayFunc(scene2);
	glutMainLoop();

	return 0;
}

