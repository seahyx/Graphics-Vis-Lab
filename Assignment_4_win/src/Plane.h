#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include "vecmath.h"
#include <cmath>
using namespace std;


class Plane: public Object3D
{
public:
	Plane(){}

	Plane( const vec3& normal , float d , Material* m):Object3D(m)
    {
        this->normal = normalize(normal);
        this->d = -d;
	}

	~Plane(){}

    /// Ray-plane intersection test.
	virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {   
        float denom = dot(this->normal, r.getDirection());
        // If the ray is very close to parallel with the plane, assume no intersection
        // also assume backface rendering is true
        if (denom < 1e-6f && denom > -1e-6f) return false;

        // Vector from plane origin to ray origin
        vec3 r_p = -d * this->normal - r.getOrigin();
        float t = dot(r_p, this->normal) / denom;
        
        // Check for tmin
        if (t < tmin || t > h.getT()) return false;

        // Closest intersection found, update hit params
        h.set(t, this->material, this->normal);
        return true;
	}

protected:
    vec3 normal;
    float d;

    
};
#endif //PLANE_H
		

