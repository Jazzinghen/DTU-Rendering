// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "ObjMaterial.h"
#include "fresnel.h"
#include "RayTracer.h"

using namespace optix;

bool RayTracer::trace_reflected(const Ray& in, const HitInfo& in_hit, Ray& out, HitInfo& out_hit) const
{
  // Initialize the reflected ray and trace it. Set out_hit.ray_ior and out_hit.trace_depth.
  // Return true if the reflected ray hit anything.
    out_hit.ray_ior = in_hit.ray_ior;
	out_hit.trace_depth  = in_hit.trace_depth + 1;
	out = Ray(in_hit.position, optix::reflect(in.direction, in_hit.geometric_normal), 0, 0.001 );
	
	trace_to_closest(out,out_hit);
 
  return out_hit.has_hit ;
}

bool RayTracer::trace_refracted(const Ray& in, const HitInfo& in_hit, Ray& out, HitInfo& out_hit) const
{
  // Initialize the refracted ray and trace it. Set out_hit.ray_ior and out_hit.trace_depth.
  // Return true if the refracted ray hit anything.
  // Remember that the function must handle total internal reflection.

	float3 refractedDirection;
	float3 dir;
    float cosi;
	float3 normal = in_hit.geometric_normal;
	float tempIor = get_ior_out(in, in_hit, dir, normal, cosi);
	//bool HasRefraction = optix::refract( refractedDirection, in.direction, in_hit.geometric_normal, tempIor/in_hit.ray_ior);//in_hit.material->ior );//todo: see get_ior stuff
	bool HasRefraction = optix::refract( refractedDirection, -dir, normal, tempIor/in_hit.ray_ior);//in_hit.material->ior );//todo: see get_ior stuff
	out = Ray(in_hit.position, refractedDirection, 0, 0.001 );
	
	if ( HasRefraction ){
		out_hit.trace_depth  = in_hit.trace_depth + 1;
		//out_hit.ray_ior = in_hit.material->ior;
	out_hit.ray_ior = tempIor;
		//out_hit.ray_ior = get_ior_out(in, in_hit, dir, normal, cosi);
		trace_to_closest(out,out_hit);
	}
  return HasRefraction ;  
}

bool RayTracer::trace_refracted(const Ray& in, const HitInfo& in_hit, Ray& out, HitInfo& out_hit, float& R) const
{
  // Initialize the refracted ray and trace it. Set out_hit.ray_ior and out_hit.trace_depth.
  // Compute the Fresnel reflectance (see fresnel.h) and return it in R.
  // Return true if the refracted ray hit anything.
  // Remember that the function must handle total internal reflection.
  
  bool res = trace_refracted(in, in_hit, out, out_hit);
  if(!res)
  {
	  R = 1;
  }
  else
  {
	  float cosi = dot( -in.direction, in_hit.geometric_normal );
	  float cost = dot( out.direction, -in_hit.geometric_normal );

	  //float rs = 
	  float3 dir;
	  float3 normal;
	  R = fresnel_R(cosi, cost, in_hit.ray_ior, out_hit.ray_ior);
	  // Cowboy home made reflectance equation
	  //R = 0.2 + 1.2*(1 - dot( -normalize( in.direction ), normalize( in_hit.geometric_normal ))) / (0.8);
  }
  return res;
}

float RayTracer::get_ior_out(const Ray& in, const HitInfo& in_hit, float3& dir, float3& normal, float& cos_theta_in) const
{
  normal = in_hit.shading_normal;
  dir = -in.direction;
  cos_theta_in = dot(normal, dir);
	if(cos_theta_in < 0.0)
	{
    normal = -normal;
		cos_theta_in = -cos_theta_in;
    return 1.0f;
  }
  const ObjMaterial* m = in_hit.material;
  return m ? m->ior : 1.0f;
}
