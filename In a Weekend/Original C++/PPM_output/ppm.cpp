/*
*   There are some things to note in that code:
*        1. The pixels are written out in rows with pixels left to right.
*        2. The rows are written out from top to bottom. 
*        3. By convention, each of the red/green/blue components range from 0.0 to 1.0.
*        4. Red goes from black to fully on from left to right, and green goes from black
*           at the bottom to fully on at the top. Red and green together make yellow so we 
*           should expect the upper right corner to be yellow.
*/

#include <iostream>

int main() {
    int nx = 200;
    int ny = 100;

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--){
        for (int i = 0; i < nx; i++) {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2;
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}