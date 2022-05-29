#include "computeClosestPointOnTheLine.h"

namespace mathem
{
	Vector3D computeClosestPointOnTheLine(Vector3D lineStart, Vector3D lineEnd, Vector3D point)
	{
		Vector3D resultClosestPoint;
		Vector3D lineVector = lineEnd - lineStart;
		lineVector.normalize();
		Vector3D pointVector = point - lineStart;

		float pointProjection = dotProduct(pointVector, lineVector);
		float lineLength = lineVector.getLength();

		if (pointProjection <= 0)
		{
			resultClosestPoint = lineStart;
		}
		else if (pointProjection >= lineLength)
		{
			resultClosestPoint = lineEnd;
		}
		else
		{
			resultClosestPoint = lineStart + lineVector * pointProjection;
		}
		return resultClosestPoint;
	}
}