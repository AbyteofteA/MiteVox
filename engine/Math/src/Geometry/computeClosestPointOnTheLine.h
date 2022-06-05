
#ifndef COMPUTECLOSESTPOINTONTHELINE_H
#define COMPUTECLOSESTPOINTONTHELINE_H

#include "engine/Math/src/Vector.h"

namespace mathem
{
	mathem::Vector3D computeClosestPointOnTheLine(Vector3D lineStart, Vector3D lineEnd, Vector3D point);
}

#endif