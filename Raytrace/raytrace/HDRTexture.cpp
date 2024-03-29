// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <iostream>
#include <optix_world.h>
#include "../SOIL/SOIL.h"
#include "int_pow.h"
#include "HDRTexture.h"

using namespace std;
using namespace optix;

void HDRTexture::load_hdr(const char* filename)
{
  data = SOIL_load_HDR_image(filename, &width, &height, &channels, SOIL_LOAD_AUTO);
  if(!data)
  {
    cerr << "Error: Could not load texture image file." << endl;
    return;
  }
  int img_size = width*height;
  fdata = new float4[img_size];
  for(int i = 0; i < img_size; ++i)
    fdata[i] = look_up(i);
  tex_handle = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, tex_handle, SOIL_FLAG_INVERT_Y);
  if(!glIsTexture(tex_handle))
    cerr << "Error: Could not construct OpenGL texture from loaded image." << endl;
}

float4 HDRTexture::look_up(unsigned int idx)
{
  if(channels != 4)
    return make_float4(0.0f);

  idx *= channels;
  int e = static_cast<int>(data[idx + 3]) - 128;
  return make_float4(convert(data[idx], e), convert(data[idx + 1], e), convert(data[idx + 2], e), 1.0f);
}

float HDRTexture::convert(unsigned char c, int e)
{
  // Implement the conversion from RGBE to floating point.
  // Note that 128 has already been subtracted from e.
  return 0.0f;
}
