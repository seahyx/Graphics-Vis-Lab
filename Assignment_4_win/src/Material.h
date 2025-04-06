#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include "vecmath.h"

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"


class Material
{
public:
	
    Material( const vec3& d_color ,const vec3& s_color=vec3(0,0,0), float s=0):
    diffuseColor( d_color),specularColor(s_color), shininess(s)
    {

    }

    virtual ~Material()
      {

      }

    virtual vec3 getDiffuseColor() const 
    { 
      return diffuseColor;
    }
      
    /// Compute diffuse and specular components of Phong lighting 
    vec3 Shade( const Ray& ray, const Hit& hit, const vec3& dirToLight, const vec3& lightColor ) 
    {
        vec3 n = normalize(hit.getNormal());
        vec3 l = normalize(dirToLight);
        vec3 r = 2.0f * n * dot(n, l) - l;

        float diffuse_dp = dot(n, l);
        diffuse_dp = diffuse_dp > 0 ? diffuse_dp : 0;
        float specular_dp = dot(r, -normalize(ray.getDirection()));
        specular_dp = specular_dp > 0 ? powf(specular_dp, this->shininess) : 0;

        vec3 I = lightColor * (this->diffuseColor * diffuse_dp + this->specularColor * specular_dp);
        return I;
    }

    void loadTexture(const char * filename)
    {
      t.load(filename);
    }

    Texture t;

 protected:
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};



#endif // MATERIAL_H
