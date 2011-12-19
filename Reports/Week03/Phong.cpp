// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "int_pow.h"
#include "Phong.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 Phong::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  // Implement Phong reflection here.
  //
  // Input:  r          (the ray that hit the material)
  //         hit        (info about the ray-surface intersection)
  //         emit       (passed on to Emission::shade)
  //
  // Return: radiance reflected to where the ray was coming from
  //
  // Relevant data fields that are available (see Lambertian.h, Ray.h, and above):
  // lights             (vector of pointers to the lights in the scene)
  // hit.position       (position where the ray hit the material)
  // hit.shading_normal (surface normal where the ray hit the material)
  // rho_d              (difuse reflectance of the material)
  // rho_s              (specular reflectance of the material)
  // s                  (shininess or Phong exponent of the material)
  //
  // Hint: (a) Call the sample function associated with each light in the scene.
  //       (b) I suggest that you convert shininess to an integer and use the
  //       function int_pow(...) which is much more efficient than the general
  //       pow(...) function.

  float3 rho_s = get_specular(hit);
  float s = get_shininess(hit);
  float3 result = Lambertian::shade(r, hit, emit);

  for(int i = 0; i < lights.size(); ++i )
  {
	  float3 dir;
	  float3 L;
	  if( lights[i]->sample(hit.position, dir, L) )
	  {
		  float dp_spec = dot( optix::reflect(-dir, hit.shading_normal), -r.direction); 
		  if( dp_spec > 0.0001 )
		  {				  
			  result += rho_s * L * int_pow<float>( dp_spec, (int)s );
		  }
	  }
	  
  }

  return result;
}
