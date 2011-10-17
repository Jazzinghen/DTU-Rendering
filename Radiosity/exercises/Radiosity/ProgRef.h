#ifndef PROGREF_H
#define PROGREF_H

#include <vector>
#include <algorithm>
#include "DataFormat.h"

MyPolygon* calcFF(Hemicube* hemicube);

MyPolygon* calcAnalyticalFF();

bool distributeEnergy(MyPolygon* maxP);

void colorReconstruction();

#endif
