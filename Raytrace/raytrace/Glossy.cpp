// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "int_pow.h"
#include "Glossy.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 Glossy::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  // Implement glossy reflection here.
  //
  // Input:  r          (the ray that hit the material)
  //         hit        (info about the ray-surface intersection)
  //         emit       (passed on to Emission::shade)
  //
  // Return: radiance reflected to where the ray was coming from
  //
  // Relevant data fields that are available (see Mirror.h and HitInfo.h):
  // max_depth          (maximum trace depth)
  // tracer             (pointer to ray tracer)
  // hit.trace_depth    (number of surface interactions previously suffered by the ray)
  //
  // Hint: Use the function shade_new_ray(...) to pass a newly traced ray to
  //       the shader for the surface it hit.
	float3 result = make_float3(0.0);

  HitInfo reflected_hit ;
  Ray reflected_ray;
  reflected_hit.has_hit = false;
  HitInfo refracted_hit;
  refracted_hit.has_hit = false;
  Ray refracted_ray;

  float R = 1.0;
  if( hit.trace_depth < max_depth ) // 
  {
	
	tracer->trace_reflected(r, hit, reflected_ray, reflected_hit);
	tracer->trace_refracted(r, hit, refracted_ray, refracted_hit, R);
  
	if( reflected_hit.has_hit )
		result += shade_new_ray(reflected_ray,reflected_hit) * R;
	else
		result += tracer->get_background() * R;
		  
	  
	if( refracted_hit.has_hit)
		result += shade_new_ray(refracted_ray,refracted_hit) * (1-R);
	else
		result += tracer->get_background() * (1-R);
	
  }
  else
  {
	  return make_float3(1.0,0.0,0.0);
  }
  //reflected_ray = refracted_ray;
  //if(reflected_ray.direction.x < 0.0) reflected_ray.direction.x = - reflected_ray.direction.x;
  //if(reflected_ray.direction.y < 0.0) reflected_ray.direction.y = - reflected_ray.direction.y;
  //if(reflected_ray.direction.z < 0.0) reflected_ray.direction.z = - reflected_ray.direction.z;
  //return normalize(-reflected_ray.direction);
  //return make_float3(R);
  return result + Phong::shade(r, hit, emit);
}
