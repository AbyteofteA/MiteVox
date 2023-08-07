#include "PhysicalMaterial.h"

#include "engine/CodeSafety/src/ensureValue.h"

#include <algorithm>

namespace mitevox
{
	PhysicalMaterial::PhysicalMaterial()
	{
		this->name = "Default Physical Material";
	}

	PhysicalMaterial::PhysicalMaterial(std::string name, float density, float restitution, float staticFriction, float dynamicFriction)
	{
		this->name = name;
		setDensity(density);
		setRestitution(restitution);
		setStaticFriction(staticFriction);
		setDynamicFriction(dynamicFriction);
	}

	void PhysicalMaterial::setDensity(float density)
	{
		this->density = std::max(0.0f, density);
	}

	void PhysicalMaterial::setRestitution(float restitution)
	{
		this->restitution = safety::ensureRange(restitution, 0.0f, 1.0f);
	}

	void PhysicalMaterial::setStaticFriction(float staticFriction)
	{
		this->staticFriction = safety::ensureRange(staticFriction, 0.0f, 1.0f);
		this->dynamicFriction = safety::ensureRange(this->dynamicFriction, 0.0f, this->staticFriction);
	}

	void PhysicalMaterial::setDynamicFriction(float dynamicFriction)
	{
		this->dynamicFriction = safety::ensureRange(dynamicFriction, 0.0f, this->staticFriction);
	}

	std::string PhysicalMaterial::getName()
	{
		return name;
	}

	float PhysicalMaterial::getDensity()
	{
		return density;
	}

	float PhysicalMaterial::getRestitution()
	{
		return restitution;
	}

	float PhysicalMaterial::getStaticFriction()
	{
		return staticFriction;
	}

	float PhysicalMaterial::getDynamicFriction()
	{
		return dynamicFriction;
	}
}