#include <math.h>
#include <vector>
#include <float.h>

#include "CGLA/Vec3f.h"
#include "Hemicube.h"
#include "Dataformat.h"
#include "ProgRef.h"

using namespace CGLA;
using namespace std;

extern vector<MyPolygon*> polygons;
extern vector<MyVertex*> vertices;

bool polygon_cmp(MyPolygon* mp1, MyPolygon* mp2)
{ 
  return sqr_length(mp1->unshot_rad) < sqr_length(mp2->unshot_rad);
}

MyPolygon* maxenergy_patch()
{
  return *std::max_element(polygons.begin(), polygons.end(), polygon_cmp);
}

MyPolygon* calcAnalyticalFF()
{
  // Reset all form factors to zero
  for(unsigned int i = 0; i < polygons.size(); i++)
    polygons[i]->formF = 0.0f;

  // Find the patch with maximum energy
  MyPolygon* maxEnergy = maxenergy_patch();

  // Calculate the form factors between the maximum patch and all other patches.
  // In this function, do it analytically [B, Sec. 11.2].

  vector<MyPolygon*>::iterator iter;
  for( iter = polygons.begin(); iter != polygons.end() ; ++iter )
  {
    if( (*iter) == maxEnergy ) 
      continue;

    Vec3f dir =  (*iter)->center - maxEnergy->center ;
    float r2 = dot(dir,dir);
    dir = normalize(dir);
    (*iter)->formF = ( max(dot((*iter)->normal, -dir),0.0f) *
                       max(dot(maxEnergy->normal, dir),0.0f) *
                       (*iter)->area )/(3.141516*r2);
    
    if((*iter)->formF < 0) 
      (*iter)->formF = 0;
  }
  
  return maxEnergy;
}

bool distributeEnergy(MyPolygon* maxP)
{
  if(maxP == 0)
  {
    cout << "\n\nwuuuuut ??\n\n";
  	return false;
  }
  // Distribute energy from the maximum patch to all other patches.
  // The energy of the maximum patch is in maxP->unshot_rad (see DataFormat.h).

  vector<MyPolygon*>::iterator iter;
  for( iter = polygons.begin(); iter != polygons.end() ; ++iter )
  {
	  if((*iter) == maxP) continue;
	  (*iter)->rad += (*iter)->formF * (*iter)->diffuse * maxP->unshot_rad * maxP->area / (*iter)->area;
    (*iter)->unshot_rad += (*iter)->formF * (*iter)->diffuse * maxP->unshot_rad * maxP->area / (*iter)->area;
  }
   
  // Set the unshot radiosity of the maximum patch to zero and return true
  maxP->unshot_rad = Vec3f(0.0,0.0,0.0);

  return true;
}

void colorReconstruction()
{

  // Set the colour of all patches by computing their radiances. 
  // (Use nodal averaging to compute the colour of all vertices 
  //  when you get to this part of the exercises.)

  for(int i = 0; i < vertices.size(); ++i)
  {
    vertices[i]->color = Vec3f(0.0,0.0,0.0);
    vertices[i]->colorcount = 0;
  }

  vector<MyPolygon*>::iterator iter;
  for( iter = polygons.begin(); iter != polygons.end() ; ++iter )
  {
	  (*iter)->color = (*iter)->rad * M_1_PI;
    for(int i = 0; i < (*iter)->vertices; ++i )
    {
      vertices[(*iter)->vertex[i]]->color += (*iter)->color;
      vertices[(*iter)->vertex[i]]->colorcount += 1;
    }
  }

  for(int i = 0; i < vertices.size(); ++i)
  {
    vertices[i]->color /= (float)vertices[i]->colorcount;
  }
 
}


void renderPatchIDs()
{
  // Render all polygons in the scene as in displayMyPolygons,
  // but set the colour to the patch index using glColor3ub.
  // Look at the Hemicube::getIndex function to see how the
  // indices are read back.

	for(int i=0;i<polygons.size();i++) {				
			if (4==polygons[i]->vertices) glBegin(GL_QUADS);
			else if (3==polygons[i]->vertices) glBegin(GL_TRIANGLES);
			else assert(false); // illegal number of vertices
		
		
		//glColor3f(polygons[i]->color[0],polygons[i]->color[1],polygons[i]->color[2]);
		
		for (int j=0;j<polygons[i]->vertices;j++) {
    
			Vec3f position = vertices[polygons[i]->vertex[j]]->position;      
      
			GLubyte i1 = (i+1) & 255;
			GLubyte i2 = ((i+1) & (255 << 8)) >> 8;
			GLubyte i3 = ((i+1) & (255 << 16)) >> 16;

			glColor3ub(i3,i2,i1);

      glVertex3f(position[0],position[1],position[2]);
		}
		glEnd();
	}

}

MyPolygon* calcFF(Hemicube* hemicube)
{
  // Reset all form factors to zero
  for(unsigned int i = 0; i < polygons.size(); i++)
  	polygons[i]->formF = 0; 
  
  // Find the patch with maximum energy
  MyPolygon* maxEnergy = maxenergy_patch();

  // Compute a normalized up vector for the maximum patch 
  // (use the patch center and one of the patch vertices, for example)
  Vec3f patchUp = normalize(maxEnergy->center - vertices[maxEnergy->vertex[0]]->position );

  // Render patch IDs to the hemicube and read back the index buffer
  hemicube->renderScene(maxEnergy->center, patchUp, maxEnergy->normal, &renderPatchIDs);
  hemicube->readIndexBuffer();


  // Compute form factors by stepping through the pixels of the hemicube
  // and calling hemicube->getDeltaFormFactor(...).
  for(int y = 0; y < hemicube->rendersize; ++y )
	for(int x = 0; x < hemicube->rendersize; ++x )
	{
	  int idx = hemicube->getIndex(x,y)-1;
	  if(idx >= 0)
		polygons[idx]->formF += hemicube->getDeltaFormFactor(x,y);
	}

  // Return the maximum patch
  return maxEnergy;
}
