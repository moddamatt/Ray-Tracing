#include "ray.h"
#include <iostream>

/* In this folder you will notice that there is a main.exe and ppm.exe
 * This is for the sole reason that I wanted to have first example from
 * the book always in reach. I find it nice to be able to go back to my 
 * "Hello World" programs to see ho far I have come. It is a personal thing.
 */

// A simple function to returns the color of the background (a simple gradient)
vec3 color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
} 

int main() {
    int nx = 200;
    int ny = 100;

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    // Details for a simplistic camera
    // Veiwing frustrum?
    vec3 lower_left_corner(-2.0, -1.0, -1.0); // Screen origin
    vec3 horizontal(4.0, 0.0, 0.0);           // Screen width
    vec3 vertical(0.0, 2.0, 0.0);             // Screen height
    vec3 origin(0.0, 0.0, 0.0);               // Camera origin

    for (int j = ny-1; j >= 0; j--){
        
        // Progress indicator. This is a handy way to track the progress of a long render,
        // and also to possibly identify a run that's stalled out due to an infinite
        // loop or other problem.
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r);
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nDone. \n";
}