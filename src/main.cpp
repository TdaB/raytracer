#include "..\Dependencies\glew-2.1.0\include\GL\glew.h"
#include "..\Dependencies\freeglut\include\GL\freeglut.h"
#include <iostream>
#include <limits>
#include <ctime>
#include <vector>
#include "objects.h"
#include "scenes.h"
#include "raytracer.h"

using namespace std;


void render_pixel(int x, int y, Color c) {
	glBegin(GL_POINTS);
	glColor3d(c.r, c.g, c.b);
	glVertex2i(x, y);
	glEnd();
}

void render_scene() {
	Raytracer r = Raytracer(get_scene());
	int width = r.scene.width;
	int height = r.scene.height;
	int bounces = r.scene.bounces;

	clock_t begin = clock();

	for (int x = -(width / 2); x < width / 2; x++) {
		for (int y = -(height / 2); y < height / 2; y++) {
			Point camera = Point(x, y, -1);
			Point pixel = Point(x, y, 0);
			Color fill = r.trace(camera, pixel, bounces);
			render_pixel(x, y, fill);
		}
	}

	glFlush();

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "RENDERING TIME: " << elapsed_secs << " SECONDS";
}

int main(int argc, char **argv) {
	Scene s = get_scene();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(s.width, s.height);
	glutCreateWindow("Faster than a speeding slug...");
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);
	// Define coords in viewing area
	gluOrtho2D(-(s.width / 2.0), s.width / 2.0, -(s.height / 2.0), s.height / 2.0);
	// 2 days of my life gone from this damn option
	glViewport(0, 0, s.width, s.height);
	glutDisplayFunc(render_scene);
	glutMainLoop();
	return 0;
}
