
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
		ColorRGBAf albedo = { 0.0f, 0.0f, 0.0f };		// (0.0 - 1.0)
		ColorRGBAf roughness = { 1.0f, 1.0f, 1.0f };	// (0.0 - 1.0)
		ColorRGBAf metallicity = { 1.0f, 1.0f, 1.0f };	// (0.0 - 1.0)
		float specularExponent = 8;						// (0.0 - 1000.0)
			// Transparancy
		ColorRGBAf filter = { 1.0f, 1.0f, 1.0f };
		float transparency = 0;		// (0.0 - 1.0)
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
	};


	/************************************************************************************
	Parse .mtl
	------------------------------------------------------------------------------------
	Application: 
	Description:
	************************************************************************************/
	inline void loadMaterial_MTL(std::string filename, void** material, fileio::FileStatus* flag)
	{
		*material = nullptr;
		*flag = fileio::FileStatus::LOADING;
		char* fileData = nullptr;

		fileio::FileStatus tmpFlag = fileio::FileStatus::LOADING;
		fileio::loadBytes(filename, (void**)&fileData, &tmpFlag);

		//Material* materialTmp = new Material();
		
		// .mtl parser is under construction //

		free(fileData);
		//(*material) = (void*)materialTmp;
		*flag = fileio::FileStatus::READY;
		return;
	}
}

#endif