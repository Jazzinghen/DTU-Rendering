// 02562 Rendering Framework
// Code from GEL (http://www.imm.dtu.dk/GEL/)
// Inspired by Nate Robins Obj loader.
// Modified by Jeppe Revall Frisvad to suit framework 
// and to use CUDA/OptiX matrix-vector types.
// Copyright (c) DTU Informatics 2011

#include <iostream>
#include <cstdio>
#include <string>
#include <optix_world.h>
#include "IndexedFaceSet.h"
#include "HitInfo.h"
#include "Object3D.h"
#include "Triangle.h"
#include "TriMesh.h"

using namespace std;
using namespace optix;

bool TriMesh::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
  uint3 face = geometry.face(prim_idx);

  // Implement ray-triangle intersection here including trilinear normal interpolation.
  // Use the function intersect_triangle(...) to get the hit info.

  return false;
}

void TriMesh::transform(const Matrix4x4& m)
{
  for(unsigned int i = 0; i < geometry.no_vertices(); ++i)
    geometry.vertex_rw(i) = make_float3(m*make_float4(geometry.vertex(i), 1.0f));
  for(unsigned int i = 0; i < normals.no_vertices(); ++i)
    normals.vertex_rw(i) = make_float3(m*make_float4(normals.vertex(i), 0.0f));
}

Aabb TriMesh::get_primitive_bbox(unsigned int prim_idx) const
{
  Aabb bbox;
  uint3 face = geometry.face(prim_idx);
  bbox.include(geometry.vertex(face.x));
  bbox.include(geometry.vertex(face.y));
  bbox.include(geometry.vertex(face.z));
  return bbox;
}

Aabb TriMesh::compute_bbox() const
{
  Aabb bbox;
  for(unsigned int i = 0; i < geometry.no_vertices(); ++i) 
    bbox.include(geometry.vertex(i));
  return bbox;
}

unsigned int TriMesh::find_material(const string& name) const
{
  for(unsigned int i = 0; i < materials.size(); ++i)
  {
    if(materials[i].name == name)
      return i;
  }
  return 0;
}

void TriMesh::compute_normals()
{		
  // By default the normal faces are the same as the geometry faces
  // and there are just as many normals as vertices, so we simply
  // copy.
  normals = geometry;

  // The normals are initialized to zero.
  for(unsigned int i = 0; i < normals.no_vertices(); ++i)
    normals.vertex_rw(i) = make_float3(0.0f);

  // For each face
  for(unsigned int i = 0; i < geometry.no_faces(); ++i)
  {
    // Compute the normal
    const uint3& f  = geometry.face(i);
    const float3& p0 = geometry.vertex(f.x);
    const float3& a  = geometry.vertex(f.y) - p0;
    const float3& b  = geometry.vertex(f.z) - p0;
    float3 face_normal = cross(a, b);
    float len = dot(face_normal, face_normal);
    if(len > 0.0f)
    face_normal /= sqrt(len);

    // Add the angle weighted normal to each vertex
    const unsigned int* fp = &f.x;
    for(int j = 0; j < 3; ++j)
    {
      const float3& p0 = geometry.vertex(fp[j]);
      float3 a = geometry.vertex(fp[(j + 1)%3]) - p0;
      float len_a = dot(a, a);
      if(len_a > 0.0f)
        a /= sqrt(len_a);
      float3 b = geometry.vertex(fp[(j + 2)%3]) - p0;
      float len_b = dot(b, b);
      if(len_b > 0.0f)
        b /= sqrt(len_b);
      float d = max(-1.0f, min(1.0f, dot(a,b)));
      normals.vertex_rw(fp[j]) += face_normal*acos(d);
    }
  }

  // Normalize all normals
  for(unsigned int i = 0; i < normals.no_vertices(); ++i)
  {
    const float3& normal = normals.vertex(i);
    float len_normal = dot(normal, normal);
    if(len_normal > 0.0f)
      normals.vertex_rw(i) /= sqrt(len_normal);
  }
}
