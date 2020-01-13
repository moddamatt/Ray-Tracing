#ifndef RAY_H
#define RAY_H
#include "vec3.h"

/*
 * By definition a ray tracer needs a ray to trace and a computation of the colors
 * seen along the ray. The function offered by Petter is given as p(t) = A + t * B.
 * 
 * p being the point along the ray in 3D space
 * A is the ray's origin
 * B is the direction of the array
 * t is a real number, a negative t allows you to see any point along the array
 *   while a positive t only allows parts in fornt of A 
 */

class ray
{
    public:
        ray() {}
        ray(const vec3& a, const vec3& b)       { A = a; B = b;}
        vec3 origin() const                     { return A; }
        vec3 direction() const                  { return B; }
        vec3 point_at_parameter(float t) const  { return A + t * B; }

        vec3 A;
        vec3 B;
};

#endif