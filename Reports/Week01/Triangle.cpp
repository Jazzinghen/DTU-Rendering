// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Triangle.h"


using namespace optix;

// defined in Plane.cpp
bool intersect_plane(const float3& normal, float d, const Ray& r, float& distance);

bool intersect_triangle(const Ray& r, 
                        const float3& v0, 
                        const float3& v1, 
                        const float3& v2, 
                        float3& n,
                        float& t,
                        float& v,
                        float& w)
{
  // Implement ray-triangle intersection here.
  // Note that OptiX also has an implementation, so you can get away
  // with not implementing this function. However, I recommend that
  // you implement it for completeness.

  float3 e0 = v1 - v0;
  float3 e1 = v2 - v0;
  n = cross( e1, e0 ) ;
  
  float d = - n.x*v0.x - n.y*v0.y - n.z*v0.z;
  
  if( !intersect_plane(n, d, r, t) ) return false;
  
  float3 OrV0 = optix::make_float3( v0.x - r.origin.x, v0.y - r.origin.y, v0.z - r.origin.z );
  
  float3 OrV0_cross_dir = cross( OrV0, r.direction );
  float dir_dot_n = dot( r.direction, n );
  
  v = -dot( OrV0_cross_dir, e1) / dir_dot_n;
  w = dot( OrV0_cross_dir, e0) / dir_dot_n;  
  n = -n;
  if( (v < 0.0) || (w < 0.0) || (v+w > 1.0) ) return false;
  return true;
}


bool Triangle::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
  // Implement ray-triangle intersection here.
  //
  // Input:  r                    (the ray to be checked for intersection)
  //         prim_idx             (index of the primitive element in a collection, not used here)
  //
  // Output: hit.has_hit          (true if the ray intersects the triangle, false otherwise)
  //         hit.dist             (distance from the ray origin to the intersection point)
  //         hit.geometric_normal (the normalized normal of the triangle)
  //         hit.shading_normal   (the normalized normal of the triangle)
  //         hit.material         (pointer to the material of the triangle)
  //        (hit.texcoord)        (texture coordinates of intersection point, not needed for Week 1)
  //
  // Return: True if the ray intersects the triangle, false otherwise
  //
  // Relevant data fields that are available (see Triangle.h)
  // r                            (the ray)
  // v0, v1, v2                   (triangle vertices)
  // (t0, t1, t2)                 (texture coordinates for each vertex, not needed for Week 1)
  // material                     (material of the triangle)
  //
  // Hint: Use the function intersect_triangle(...) to get the hit info.
  //       Note that you need to do scope resolution (optix:: or just :: in front
  //       of the function name) to choose between the OptiX implementation and
  //       the function just above this one.

  float3 normal;
  float v, w, distance;
  if( !(::intersect_triangle(r, v0, v1, v2, normal, distance, v, w )) )
  {
      return hit.has_hit = false;
  }
  
  hit.has_hit = true;
  hit.dist = distance;
  hit.geometric_normal = normalize( normal );
  hit.shading_normal = normalize( normal );
  hit.material = &get_material();
  hit.position = r.origin + distance * r.direction;
  return true;
}

void Triangle::transform(const Matrix4x4& m) 
{ 
  v0 = make_float3(m*make_float4(v0, 1.0f)); 
  v1 = make_float3(m*make_float4(v1, 1.0f)); 
  v2 = make_float3(m*make_float4(v2, 1.0f)); 
}

Aabb Triangle::compute_bbox() const
{
  Aabb bbox;
  bbox.include(v0);
  bbox.include(v1);
  bbox.include(v2);
  return bbox;
}
