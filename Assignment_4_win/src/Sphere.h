#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include "vecmath.h"
#include <cmath>

#include <iostream>
using namespace std;

class Sphere: public Object3D
{
public:
    Sphere()
    { 
        //unit ball at the center
        this->center = vec3(0,0,0);
        this->radius = 1.0;
    }

    Sphere( vec3 center , float radius , Material* material ):Object3D(material)
    {
        this->center = center;
        this->radius = radius;
    }
    

    ~Sphere(){}

    /// Ray-sphere intersection test.
    virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {
        // Sphere-to-ray-origin vector
        vec3 rel_sphere_center = r.getOrigin() - this->center;

        // Dot of sphere-to-ray-origin vector and ray direction
        // Negative if ray is pointing in the general direction of the sphere
        float b = dot(rel_sphere_center, r.getDirection());

        // Square of distance between sphere center and ray origin subtracted by square of radius
        // Positive if ray origin is outside of sphere
        float c = dot(rel_sphere_center, rel_sphere_center) - this->radius * this->radius;

        // No intersection if ray origin is outside sphere and pointing away from sphere
        if (c > 0.0f && b > 0.0f) return false;

        // Calculate discriminant to determine whether an intersection will happen
        float discr = b * b - c;
        if (discr < 0.0f) return false; // Ray misses sphere

        // Compute smallest t value of intersection
        float t = -b - sqrtf(discr);

        // Check against t min
        if (t < tmin || t > h.getT()) return false;

        // Closest intersection found, updating hit parameters
        // Normal is direction of hit point from sphere center
        vec3 normal = normalize(r.pointAtParameter(t) - this->center);
        h.set(t, this->material, normal);
        return true;
    }

protected:
    vec3 center;
    float radius;
  

};


#endif
