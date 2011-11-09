// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <iostream>
#include "my_glut.h"
#include <optix_world.h>
#include "../SOIL/SOIL.h"
#include "Texture.h"

using namespace std;
using namespace optix;

void Texture::load(const char* filename)
{
  SOIL_free_image_data(data);
  data = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_AUTO);
  if(!data)
  {
    cerr << "Error: Could not load texture image file." << endl;
    return;
  }
  int img_size = width*height;
  delete [] fdata;
  fdata = new float4[img_size];
  for(int i = 0; i < img_size; ++i)
    fdata[i] = look_up(i);
  tex_handle = SOIL_create_OGL_texture(data, width, height, channels, tex_handle, SOIL_FLAG_INVERT_Y);
  tex_target = GL_TEXTURE_2D;
}

void Texture::load(GLenum target, GLuint texture)
{
  glBindTexture(target, texture);    
  glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &height);
  delete [] fdata;
  fdata = new float4[width*height];
  glGetTexImage(target, 0, GL_RGBA, GL_FLOAT, &fdata[0].x);
  tex_handle = texture;
  tex_target = target;
}

float4 sample_offset( const float4* fdata, int width, int height, const float3& texcoord, int dx, int dy)
{
  int tx = (int) (texcoord.x * width)+dx;
  int ty = (int) (texcoord.y * height)+dy;
  
  // modulo operation 
  if( tx < 0 )
      tx = width + tx % width;
  else
      tx = tx % width;

  if( ty < 0 )
      ty = height + ty % height;
  else
      ty = ty % width;

  if (fdata)  return fdata[ tx + ty * width ];

  return make_float4( 1.0, 1.0, 0.0, 1.0 );
}

float4 Texture::sample_nearest(const float3& texcoord) const
{

  // Use texcoord.x and texcoord.y as uv-coordinates to
  // look-up the nearest texel in the fdata array which holds
  // the texture in float4 format.
  return sample_offset(&fdata[0], width, height, texcoord,0,0);
}

float4 Texture::sample_linear(const float3& texcoordinates) const
{
    //return make_float4(1.0,1.0,0.0,1.0);
  //return sample_nearest(texcoordinates);

  float3 texcoord = texcoordinates;
  // Use texcoord.x and texcoord.y as uv-coordinates. Look-up 
  // the four nearest neighbors in the fdata array and use
  // bilinear interpolation to find the texture value.
  float4 bottomleft = sample_offset(&fdata[0], width, height, texcoord,0,0);
  float4 bottomright = sample_offset(&fdata[0], width, height, texcoord,1,0);
  float4 topright = sample_offset(&fdata[0], width, height, texcoord,1,1);
  float4 topleft = sample_offset(&fdata[0], width, height, texcoord,0,1);
  
  //float dx = texcoord.x*width - (float) ((int) (texcoord.x*(float)width));
  //float dy = texcoord.y*height - (float) ((int) (texcoord.y*(float)height));

  double dummy;
  float dx = modf(texcoord.x*(float)width,&dummy);
  float dy = modf(texcoord.y*(float)height,&dummy);

  if (dx < 0.0) dx = 1.0 + dx;
  if (dy < 0.0) dy = 1.0 + dy;
  
  return bottomleft * (1.0-dx)*(1.0-dy)
      + bottomright * dx*(1.0-dy)
      + topleft * dy*(1.0-dx)
      + topright * dx*dy;
}

float4 Texture::look_up(unsigned int idx) const
{
  idx *= channels;
  switch(channels)
  {
  case 1: 
  {
    float v = convert(data[idx]);
    return make_float4(v, v, v, 1.0f);
  }
  case 2: 
    return make_float4(convert(data[idx]), convert(data[idx]), convert(data[idx]), convert(data[idx + 1]));
  case 3: 
    return make_float4(convert(data[idx]), convert(data[idx + 1]), convert(data[idx + 2]), 1.0f);
  case 4: 
    return make_float4(convert(data[idx]), convert(data[idx + 1]), convert(data[idx + 2]), convert(data[idx + 3]));
  }
  return make_float4(0.0f);
}

float Texture::convert(unsigned char c) const
{
  return (c + 0.5f)/256.0f;
}
