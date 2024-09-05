#include <iostream>

#include "camera/camera.h"
#include "hittable_objects/hittable_list.h"
#include "hittable_objects/sphere.h"

using namespace std;

int main()
{

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(.2,.2,.2));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto glass = make_shared<dielectric>(1.5);
    auto _metal = make_shared<metal>(color(0.8, 0.6, 0.2), 0.3);
    auto _diffuse_light = make_shared<diffuse_light>(color(5,5,5));

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, glass));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.25, _metal));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, _diffuse_light));

    // Image 
  
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 1600;

    camera cam(aspect_ratio, image_width);

    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 50;
    cam.lookfrom = point3(0, 0, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 1.0;
    cam.focus_dist = 2;

    cam.background = color(0.3, 0.8, 0.5);

    cam.render(world);

}