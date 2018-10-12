#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <limits>
#include <ctime>
#include <vector>
#include "objects.h"
#include "utils.h"
#include "scenes.h"
#include "raytracer.h"

using namespace std;


void render_pixel(int x, int y, Color c) {
	glBegin(GL_POINTS);
	glColor3d(c.r, c.g, c.b);
	glVertex2i(x, y);
	glEnd();
}

void render_scene(Scene s) {
    clock_t begin = clock();
    
    Raytracer r = Raytracer(s);
    for (int x = -(s.width / 2); x < s.width / 2; x++) {
        for (int y = -(s.height / 2); y < s.height / 2; y++) {
            Point pixel = Point(x, y, 0);
            Color fill = r.trace(s.camera, pixel, s.bounces);
            render_pixel(x, y, fill);
        }
    }
    glFlush();
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "RENDERING TIME: " << elapsed_secs << " SECONDS";
}

void init_glut_and_render(int argc, char **argv, Scene s) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(s.width, s.height);
    glutCreateWindow("Dat Raytracer");
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    // Define coords in viewing area
    gluOrtho2D(-(s.width / 2.0), s.width / 2.0, -(s.height / 2.0), s.height / 2.0);
    // 2 days of my life gone from this damn option
    glViewport(0, 0, s.width, s.height);
    glutDisplayFunc(render_scene(s));
    glutMainLoop();
}

int main(int argc, char **argv) {
    init_glut_and_render(argc, argv, get_scene());
	return 0;
}

