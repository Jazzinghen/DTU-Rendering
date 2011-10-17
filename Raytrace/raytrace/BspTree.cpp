// 02562 Rendering Framework
// Inspired by BSP tree in GEL (http://www.imm.dtu.dk/GEL/)
// BSP tree in GEL originally written by Bent Dalgaard Larsen.
// This file written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <vector>
#include <optix_world.h>
#include "AccObj.h"
#include "Object3D.h"
#include "HitInfo.h"
#include "BspTree.h"

using namespace std;
using namespace optix;

namespace
{
  const float f_eps = 1.0e-6f;  
  const float d_eps = 1.0e-12f;
}

BspTree::~BspTree()
{
  delete_node(root);
}

void BspTree::init(const vector<Object3D*>& geometry, const std::vector<const Plane*>& scene_planes)
{
  root = new BspNode;
  Accelerator::init(geometry, scene_planes);
  for(unsigned int i = 0; i < geometry.size(); ++i)
    bbox.include(geometry[i]->compute_bbox());
  vector<AccObj*> objects = primitives;
  subdivide_node(*root, bbox, 0, objects);
}

bool BspTree::closest_hit(Ray& r, HitInfo& hit) const
{
  // Call closest_plane(...) and intersect_node(...) instead of
  // Accelerator::closest_hit(...) to use the BSP tree.
  return Accelerator::closest_hit(r, hit);
}

bool BspTree::any_hit(Ray& r, HitInfo& hit) const
{
  // Call any_plane(...) and intersect_node(...) instead of
  // Accelerator::any_hit(...) to use the BSP tree.
  return Accelerator::any_hit(r, hit);
}

void BspTree::subdivide_node(BspNode& node, Aabb& bbox, unsigned int level, vector<AccObj*>& objects) 
{
  // Use recursion with this function to build the BSP tree.
}

bool BspTree::intersect_node(Ray& ray, HitInfo& hit, const BspNode& node) const 
{
  // Use recursion with this function to compute ray-scene intersection
  // using the BSP tree.

  return false;
}

void BspTree::delete_node(BspNode *node) 
{
  if(node)
  {
    if(node->left)
      delete_node(node->left);
    if(node->right)
      delete_node(node->right);
    delete node;
  }
}
