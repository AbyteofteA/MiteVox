#ifndef MITEVOX_TRANSFORM_H
#define MITEVOX_TRANSFORM_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

#include <cstdint>

namespace mitevox
{
#ifdef HUGE_SPACE
	typedef int64_t RegionCoordType;
#else
	typedef int32_t RegionCoordType;
#endif

	typedef mathem::Vector<RegionCoordType, 3> RegionCoord;

	/// <summary>
	/// The Transform splits coordinate system into cubic regions and stores region coordinate and a transform within the region.
	/// This allows to compute precise relative transforms for rendering and physics calculations.
	/// 
	/// The idea behind the Transform is inspired by this presentation: https://www.gamedevs.org/uploads/the-continuous-world-of-dungeon-siege.pdf
	/// </summary>
	class Transform
	{
	public:

		static const float REGION_SIZE;
		static const float RECIPROCAL_REGION_SIZE;
		static const float REGION_HALF_SIZE;
		static const float RECIPROCAL_REGION_HALF_SIZE;

		RegionCoord regionCoord;
		mathem::GeometryTransform localTransform;

		void init();
		void reset();

		/// <summary>
		/// Update coordinates such that localTransform going beyond REGION_SIZE changes regionCoord.
		/// </summary>
		void updateCoordinates();

		mathem::GeometryTransform* computeRelativeTransform(Transform* originTransform);
		void applyTo(Transform* otherTransform);

		mathem::GeometryTransform* getRelativeTransform();
		mathem::Vector3D& getPosition();
		mathem::Quaternion& getOrientation();
		RegionCoord& getRegionCoord();

		void setPosition(mathem::Vector3D position);
		void setPosition(RegionCoord regionCoord, mathem::Vector3D position);
		void addPosition(mathem::Vector3D position);
		void addPosition(RegionCoord regionCoord, mathem::Vector3D position);
		void setOrientation(mathem::Quaternion quaternion);
		void addOrientation(mathem::Quaternion quaternion);
		void addOrientation(mathem::Vector3D degrees);
		void addOrientationRadians(mathem::Vector3D radians);

	private:

		mathem::GeometryTransform relativeTransform;

		void updateCoordinatesHelper(RegionCoord& regionCoord, mathem::Vector3D& localTranslation);
	};
}

#endif