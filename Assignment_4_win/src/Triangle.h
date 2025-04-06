#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include "vecmath.h"
#include <cmath>
#include <iostream>

using namespace std;

class Triangle: public Object3D
{
public:
	Triangle();
    ///@param a b c are three vertex positions of the triangle
	Triangle( const vec3& a, const vec3& b, const vec3& c, Material* m):Object3D(m)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        hasTex = false;
	}

    /// Ray-triangle intersection test
	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin)
    {
        vec3 origin = ray.getOrigin();
        vec3 dir = ray.getDirection();
        
        mat3 A = mat3(a[0] - b[0], a[0] - c[0], dir[0],
                      a[1] - b[1], a[1] - c[1], dir[1],
                      a[2] - b[2], a[2] - c[2], dir[2]);
        
        mat3 betaMat = mat3(a[0] - origin[0], a[0] - c[0], dir[0],
                            a[1] - origin[1], a[1] - c[1], dir[1],
                            a[2] - origin[2], a[2] - c[2], dir[2]);
        
        mat3 gammaMat = mat3(a[0] - b[0], a[0] - origin[0], dir[0],
                             a[1] - b[1], a[1] - origin[1], dir[1],
                             a[2] - b[2], a[2] - origin[2], dir[2]);
        
        mat3 tMat = mat3(a[0] - b[0], a[0] - c[0], a[0] - origin[0],
                         a[1] - b[1], a[1] - c[1], a[1] - origin[1],
                         a[2] - b[2], a[2] - c[2], a[2] - origin[2]);
        
        float gamma = determinant(gammaMat) / determinant(A);
        float beta = determinant(betaMat) / determinant(A);
        float alpha = 1.0f - beta - gamma;
        float t = determinant(tMat) / determinant(A);
        
        if (beta+gamma > 1 || beta < 0 || gamma < 0)
        {
            return false;
        }
        
        if ( t>tmin && t<hit.getT())
        {
            vec3 n = normalize(alpha * normals[0] + beta*normals[1] + gamma * normals[2]);
            hit.set(t, material, n);
            
            //setting texture coordinate
            vec2 tex = (alpha * texCoords[0]+ beta*texCoords[1]+ gamma * texCoords[2]);
            hit.setTexCoord(tex);
            
            return true;
        }
        else
        {
            return false;
        }
    }
    
	bool hasTex;
	vec3 normals[3];
	vec2 texCoords[3];
    
protected:
    vec3 a;
    vec3 b;
    vec3 c;

};

#endif //TRIANGLE_H
