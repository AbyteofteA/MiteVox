
#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include <string>

namespace render
{
	class Material
	{
	public:
		std::string name;

		unsigned int MaterialID;

		char isUploaded = 0;

			// Color and shading
		ColorRGBAf ambient;			// (0.0 - 1.0)
		ColorRGBAf diffuse;			// (0.0 - 1.0)
		ColorRGBAf specular;		// (0.0 - 1.0)
		float specularExponent;		// (0.0 - 1000.0)
			// Transparancy
		ColorRGBAf filter;
		float transparency;			// (0.0 - 1.0)
		float opticalDensity;		// (0.001 - 10.0)
		float reflectionSharpness;	// (0.0 - 1000.0)

		char illuminationModel;		// (0 - 10)

		//-------------------------------------------

		// TOFIX: they should be pointers

		fileio::Image albedoMap;
		unsigned int ambientMapID;

		fileio::Image roughnessMap;
		unsigned int diffuseMapID;

		fileio::Image metallicMap;
		unsigned int specularMapID;

		//-------------------------------------------

		fileio::Image normalMap;
		unsigned int normalMapID;

		fileio::Image opacityMap;
		unsigned int opacityMapID;

		fileio::Image glowMap;
		unsigned int glowMapID;

		Cubemap reflectionMap;
	};


	/************************************************************************************
	Parse .mtl
	------------------------------------------------------------------------------------
	Application: 
	Description:
	************************************************************************************/
	inline void loadMaterial_MTL(std::string filename, void** material, char* flag)
	{
		*material = nullptr;
		*flag = 0;
		char* fileData = nullptr;

		char tmpFlag = 0;
		fileio::loadBytes(filename, (void**)&fileData, &tmpFlag);

		//Material* materialTmp = new Material();
		
		// .mtl parser is under construction //

		free(fileData);
		//(*material) = (void*)materialTmp;
		*flag = 1;
		return;
	}
}

#endif