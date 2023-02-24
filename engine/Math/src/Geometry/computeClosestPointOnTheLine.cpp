#include "computeClosestPointOnTheLine.h"

namespace mathem
{
	Vector3D computeClosestPointOnTheLine(Vector3D lineStart, Vector3D lineEnd, Vector3D point)
	{
		Vector3D lineVector = lineEnd - lineStart;
		float lineLength = lineVector.getLength();
		lineVector.normalize();

		Vector3D pointVector = point - lineStart;
		float pointProjection = pointVector * lineVector;

		Vector3D resultClosestPoint;
		if (pointProjection <= 0.0f)
		{
			resultClosestPoint = lineStart;
		}
		else if (pointProjection >= lineLength)
		{
			resultClosestPoint = lineEnd;
		}
		else
		{
			resultClosestPoint = lineVector * pointProjection;
			resultClosestPoint += lineStart;
		}
		return resultClosestPoint;
	}
}