#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vecmath.h"
#include "Object3D.h"

class Transform: public Object3D
{
public: 
    Transform(){}

    Transform( const mat4& m, Object3D* obj ):o(obj)
    {
      this->m = m;
      // this->o = obj;
    }

    ~Transform()
    {

    }
    /// Transforms the ray into the object's local coordindate frame
    virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {
        vec4 origin4 = vec4(r.getOrigin(), 1.0f);
        vec4 direction4 = vec4(r.getDirection(), 0.0f);
        vec4 newOrigin = inverse(m) * origin4;
        vec4 newDirection = inverse(m) * direction4;
        vec3 ori = vec3(newOrigin[0], newOrigin[1], newOrigin[2]);
        vec3 dir = vec3(newDirection[0], newDirection[1], newDirection[2]);
        
        Ray transformedRay = Ray(ori, dir);
        if (o->intersect(transformedRay, h, tmin))
        {
            //transforming normals
            vec4 normal4 = vec4(h.getNormal(), 0.0f);
            vec4 newNormal = (transpose(inverse(m)) * normal4); //(M-1Transposed)
            vec3 normal = vec3(newNormal[0], newNormal[1], newNormal[2]);
            h.set(h.getT(), h.getMaterial(), normal);
            return true;
        }

        return false;  
    }

 protected:
    Object3D* o; //un-transformed object
    mat4 m;
};

#endif //TRANSFORM_H
