#ifndef PHYSICALMATERIAL_H
#define PHYSICALMATERIAL_H

#include <string>

namespace mitevox
{
	class PhysicalMaterial
	{
	public:

		PhysicalMaterial();
		PhysicalMaterial(std::string name, float density, float restitution, float staticFriction, float dynamicFriction);

		void setDensity(float density);
		void setRestitution(float restitution);
		void setStaticFriction(float staticFriction);
		void setDynamicFriction(float dynamicFriction);

		std::string getName();

		float getDensity();
		float getRestitution();
		float getStaticFriction();
		float getDynamicFriction();

	private:

		std::string name;

		float density = 1.0f;			// kg/m^3
		float restitution = 0.0f;		// 
		float staticFriction = 1.0f;	// 
		float dynamicFriction = 1.0f;	// 
	};
}

#endif