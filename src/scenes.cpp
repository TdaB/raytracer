#include "objects.h"
#include <vector>

using namespace std;


Scene scene1() {
	int width = 800;
	int height = 600;
    
    vector<Light> lights = {
        Light(Point(0, 400, 400), Color(1, 1, 1)),
    };
    
    vector<Sphere> spheres = {
        Sphere(Point(-200, -150, 100), 80, Properties(Color(1, 0, 0), .01, .5, .5, 40, .9, 0)),
        Sphere(Point(200, -150, 100), 80, Properties(Color(.2, .7, .4), .01, .5, .5, 12, .9, 0)),
        Sphere(Point(-200, 300, 300), 80, Properties(Color(.2, .4, .5), .01, .5, .5, 12, .9, 0)),
        Sphere(Point(0, 0, 400), 200, Properties(Color(1, 1, 0), .01, .4, .1, 40, .3, 0))
    };

	vector<Plane> planes;
    
    int bounces = 15;
    
    return Scene(width, height, spheres, planes, lights, bounces);
}

Scene scene2() {
	int width = 800;
	int height = 600;
    
    vector<Light> lights = {
        Light(Point(0, 0, 0), Color(1, 1, 1)),
        //Light(Point(-300, 0, 500), Color(1, 1, 1)),
        //Light(Point(500, 0, 420), Color(1, 1, 1))
    };
    
    vector<Sphere> spheres = {
        Sphere(Point(-100, -100, 220), 100, Properties(Color(1, 0, 0), .01, .5, .5, 40, .9, 3)),
        Sphere(Point(100, -100, 220), 100, Properties(Color(1, 0, 0), .01, .5, .5, 40, .9, 2)),
        Sphere(Point(-100, 100, 220), 100, Properties(Color(1, 0, 0), .01, .5, .5, 40, .9, 1.1)),
        Sphere(Point(100, 100, 320), 100, Properties(Color(1, 0, 0), .01, .5, .5, 40, .9, 1.5)),
        Sphere(Point(200, -150, 350), 80, Properties(Color(.2, .7, .4), .01, .5, .5, 12, .9, 0)),
        Sphere(Point(-110, 130, 250), 80, Properties(Color(.2, .4, .5), .01, .5, .5, 12, .9, 0)),
        Sphere(Point(-100, 0, 430), 100, Properties(Color(1, 0, 0), .01, .5, .5, 50, .7, 0)),
        Sphere(Point(0, 100, 1600), 1000, Properties(Color(1, 1, 1), .01, .4, .1, 40, 0, 0)),
		Sphere(Point(200, 200, 350), 80, Properties(Color(1, 0, 1), .01, .5, .5, 50, .9, 0))
    };

	vector<Plane> planes;
    
    int bounces = 15;
    
    return Scene(width, height, spheres, planes, lights, bounces);
}

Scene scene3() {
	int width = 300;
	int height = 300;

	vector<Light> lights = {
		Light(Point(0, 0, 0), Color(1, 1, 1)),
	};

	vector<Sphere> spheres;

	vector<Plane> planes = {
		Plane(Point(-100, 100, 1000), Point(-100, -100, 1300), Point(100, 0, 1500), Properties(Color(1, 0, 1), .01, .5, 0, 40, 0))
	};

	int bounces = 10;

	return Scene(width, height, spheres, planes, lights, bounces);
}

Scene get_scene() {
	return scene3();
}
