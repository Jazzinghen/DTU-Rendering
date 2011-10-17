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

  // Ray( float3 origin_, float3 direction_, unsigned int ray_type_, float tmin_, float tmax_ = RT_DEFAULT_MAX )
  float3 lightVec = light_pos - pos;
  dir = normalize( lightVec );
  //float temp = sqrt(dot(lightVec,lightVec)); 
  Ray r( pos, normalize( lightVec ), 0, 0.01, sqrt(dot(lightVec,lightVec)) );
  HitInfo hit;
  tracer->trace_to_any(r,hit);

  float dp = dot(hit.shading_normal,dir);
  if( dp > 0 )
  {

  }

  if( hit.has_hit )
  {
      L = optix::make_float3(0.0,0.0,0.0);
  }
  else
  {
      // Lets consider that the radiance is proportionnal to the invert
      // of the squarred distance.
      L = intensity / dot( lightVec, lightVec ) * 0.1; 	  
  }

  return true; 
  return !hit.has_hit; 
}

bool PointLight::emit(Ray& r, HitInfo& hit, float3& Phi) const
{
  // Sample ray direction and create ray
  Ray r2(r); // TODO !
  // Trace ray
  tracer->trace_to_any(r2,hit);
  // If a surface was hit, compute Phi and return true
  
  // TODO !
  
  return false;
}
