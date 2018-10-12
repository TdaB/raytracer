#include "objects.h"
#include <vector>

using namespace std;


Scene get_scene() {
    return scene1();
}

Scene scene1() {
    Point camera = Point(0, 0, -800);
    
    vector<Light> lights = {
        Light(Point(0, 1000, 400), Color(1, 1, 1)),
        Light(Point(-300, 1000, 300), Color(1, 1, 1)),
        Light(Point(0, 0, 150), Color(1, 1, 1))
    };
    
    vector<Sphere> spheres = {
        Sphere(Point(-200, -150, 100), 80, Color(1, 0, 0), .05, .5, .5, .9, 40, 0),
        Sphere(Point(200, -150, 100), 80, Color(.2, .7, .4), .05, .5, .5, .9, 12, 0),
        Sphere(Point(-200, 300, 300), 80, Color(.2, .4, .5), .05, .5, .5, .9, 12, 0),
        Sphere(Point(0, 0, 400), 200, Color(1, 1, 0), .05, .4, .1, .3, 40, 0)
    };
    
    int bounces = 15;
    
    return Scene(800, 600, camera, spheres, lights, bounces);
}

Scene scene2() {
    Point camera = Point(0, 0, -800);
    
    vector<Light> lights = {
        Light(Point(0, 0, 0), Color(1, 1, 1)),
        Light(Point(-300, 0, 500), Color(1, 1, 1)),
        Light(Point(500, 0, 420), Color(1, 1, 1))
    };
    
    vector<Sphere> spheres = {
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
    
    int bounces = 15;
    
    return Scene(800, 600, camera, spheres, lights, bounces);
}