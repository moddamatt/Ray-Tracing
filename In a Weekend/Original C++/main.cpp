#include "vec3.h"
#include <iostream>

// In this folder you will notice that there is a main.exe and ppm.exe
// This is for the sole reason that I wanted to have first example from
// the book always in reach. I find it nice to be able to go back to my 
// "Hello World" programs to see ho far I have come. It is a personal thing.

int main() {
    int nx = 200;
    int ny = 100;

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--){
        
        // Progress indicator. This is a handy way to track the progress of a long render,
        // and also to possibly identify a run that's stalled out due to an infinite
        // loop or other problem.
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < nx; i++) {
            vec3 color(double(i)/nx, double(j)/ny, 0.2);
            color.write_color(std::cout);
        }
    }

    std::cerr << "\nDone. \n";
}