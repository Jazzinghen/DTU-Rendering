// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <vector>
#include <optix_world.h>
#include "AccObj.h"
#include "Object3D.h"
#include "Plane.h"
#include "HitInfo.h"
#include "Accelerator.h"

using namespace std;
using namespace optix;

Accelerator::~Accelerator()
{
  for(unsigned int i = 0; i < primitives.size(); ++i)
    delete primitives[i];
}

void Accelerator::init(const vector<Object3D*>& geometry, const vector<const Plane*>& scene_planes)
{
  for(unsigned int i = 0; i < geometry.size(); ++i)
  {
    Object3D* obj = geometry[i];
    unsigned int no_of_prims = primitives.size();
    primitives.resize(no_of_prims + obj->get_no_of_primitives());
    for(unsigned int j = 0; j < obj->get_no_of_primitives(); ++j)
      primitives[j + no_of_prims] = new AccObj(obj, j);
  }
  planes = scene_planes;
}

bool Accelerator::closest_hit(optix::Ray& r, HitInfo& hit) const
{
  closest_plane(r, hit);

  // Use simple looping to find the closest intersection (if any).
  // Compute hit.position if the ray hit something.
  //
  // Input:  r    (the ray to be checked for intersection)
  // Output: hit  (info about the ray-surface intersection)
  // Return: True if the ray intersects something, false otherwise
  //
  // Relevant data fields that are available (see Accelerator.h and OptiX math library reference)
  // primitives   (array of primitive objects)
  // r.origin     (ray origin)
  // r.direction  (ray direction)
  // r.tmax       (maximum intersection distance allowed)
  //
  // Hint: Call the intersect(...) function for each primitive 
  //       object in the scene.

  
	HitInfo tempHit = hit;
	float closestDist = -1.; //dirty :p

	if(hit.has_hit)
	{
		tempHit = hit;
		closestDist = hit.dist;
	}

	for( int i = 0; i < primitives.size(); ++i )
	{
		//intersect(const optix::Ray& r, HitInfo& hit, unsigned int prim_idx) const;
		if( primitives[i]->geometry->intersect( r, tempHit, i ) )
		{
			
			if( (tempHit.has_hit) && (tempHit.dist < closestDist) )
			{
				hit = tempHit;
				closestDist = tempHit.dist;
			}
			else if((tempHit.has_hit) && closestDist < 0. )
			{
				hit = tempHit;
				closestDist = tempHit.dist;
			}
		}
	}

  return hit.has_hit;
  
}

bool Accelerator::any_hit(optix::Ray& r, HitInfo& hit) const
{
  if(!any_plane(r, hit))
  {
    // Loop over the primitive objects to check for intersection 
    // until one or none is found.
    //
    // Input:  r    (the ray to be checked for intersection)
    // Output: hit  (info about the ray-surface intersection)
    // Return: True if the ray intersects something, false otherwise
    //
    // Relevant data fields that are available (see Accelerator.h)
    // primitives   (array of primitive objects)
    //
    // Hint: Use the intersect(...) function of the primitives.
	
	  hit.has_hit = false;
	  for ( int i = 0; i < primitives.size(); ++i)
	  {
			if( primitives[i]->geometry->intersect( r, hit, i ) )
			{	
				hit.has_hit = true;
				return true;
			}
	  }

  }
  return hit.has_hit;
}

void Accelerator::closest_plane(Ray& r, HitInfo& hit) const
{
  // Call the intersect(...) function for all the planes separately.
  //
  // Input:  r    (the ray to be checked for intersection)
  // Output: hit  (info about the ray-surface intersection)
  //
  // Relevant data fields that are available (see Accelerator.h)
  // planes       (array of planes in the scene)
	
	/*
	HitInfo tempHit = hit;
	float closestDist = 99999999.0; //dirty :p
	for( int i = 0; i < planes.size(); ++i )
	{
		//intersect(const optix::Ray& r, HitInfo& hit, unsigned int prim_idx) const;
		if( planes[i]->intersect( r, tempHit, i ) )
		{
			if( ( !tempHit.has_hit ) || (tempHit.dist < closestDist) )
			{
				hit = tempHit;
				closestDist = tempHit.dist;
			}
		}
	}

	if( !tempHit.has_hit )
		hit.has_hit = false;
	*/

	float closestDist = 99999999.0; //dirty :p
	for( int i = 0; i < planes.size(); ++i )
	{
		//intersect(const optix::Ray& r, HitInfo& hit, unsigned int prim_idx) const;
		if( planes[i]->intersect( r, hit, i ) )
		{
			if( ( !hit.has_hit ) || (hit.dist < closestDist) )
			{
				closestDist = hit.dist;
			}
		}
	}

	if( !hit.has_hit )
		hit.has_hit = false;

}

bool Accelerator::any_plane(Ray& r, HitInfo& hit) const
{
  // Check for intersection with planes separately.
  //
  // Input:  r    (the ray to be checked for intersection)
  // Output: hit  (info about the ray-surface intersection)
  // Return: True if the ray intersects something, false otherwise
  //
  // Relevant data fields that are available (see Accelerator.h)
  // planes       (array of planes in the scene)

	for( int i = 0; i < planes.size(); ++i )
	{
		//intersect(const optix::Ray& r, HitInfo& hit, unsigned int prim_idx) const;
		if( planes[i]->intersect( r, hit, i ) )
		{		
			return true;
		}
	}
	return false;
}
