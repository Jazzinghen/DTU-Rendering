// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "mt_random.h"
#include "PointLight.h"

using namespace optix;



bool PointLight::sample(const float3& pos, float3& dir, float3& L) const
{
  // Compute output and return value given the following information.
  //
  // Input:  pos (the position of the geometry in the scene)
  //
  // Output: dir (the direction toward the light)
  //         L   (the radiance received from the direction dir)
  //
  // Return: true if not in shadow
  //
  // Relevant data fields that are available (see Directional.h and Light.h):
  // shadows    (on/off flag for shadows)
  // tracer     (pointer to ray tracer)
  // light_pos  (position of the point light)
  // intensity  (intensity of the emitted light)
  //
  // Hint: Construct a shadow ray using the Ray datatype. Trace it using the
  //       pointer to the ray tracer.
  dir = normalize(light_pos-pos);
  //Kepler's inverse square law
  L = intensity / (dot(light_pos-pos,light_pos-pos));


  if(shadows)
  {
	  Ray r(pos, dir, 0, 0.0001, RT_DEFAULT_MAX);
	  HitInfo info;
	  info.trace_depth=0;
	  return !tracer->trace_to_any(r, info);
  }

  return true;
}

bool PointLight::emit(Ray& r, HitInfo& hit, float3& Phi) const
{
  // Sample ray direction and create ray
	float3 direction;
	do{
		direction.x = ((rand()%1000)/500.0f)-1;
		direction.y = ((rand()%1000)/500.0f)-1;
		direction.z = ((rand()%1000)/500.0f)-1;
	}while(dot(direction,direction)>1);

  // Trace ray
	Ray rr(light_pos, normalize(direction),0,0.001);
	r=rr;
	if(tracer->trace_to_closest(r,hit))
	{
		// If a surface was hit, compute Phi and return true
		Phi = intensity * 4 * M_PIf;
		return true;
	}
	return false;
}
