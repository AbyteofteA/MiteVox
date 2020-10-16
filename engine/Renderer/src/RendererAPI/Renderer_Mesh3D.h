
#ifndef RENDERER_MESH3D_H
#define RENDERER_MESH3D_H

#include <cstdlib>
#include <string>

namespace render
{
	struct Graphics_Polygon
	{
		long* p = nullptr;
		long* t = nullptr;
		long* n = nullptr;
		ColorRGBA* color = nullptr;
		int amOfVertices = 0;
	};

	struct Mesh3D
	{
		char type = 0;

		char isUploaded = 0;

		mathem::Point3D* v = nullptr;
		long amOfVertices = 0;
		unsigned int vertexBufferID = -1;

		mathem::Point3D vMax, vMin;

		mathem::Vector3D* vn = nullptr;
		long amOfNormals = 0;

		mathem::Point3D* vt = nullptr;
		long amOfTextureCoords = 0;

		Graphics_Polygon* f = nullptr;
		long amOfFaces = 0;
	};


	double* readFloats3(char* str, int& indx);

	mathem::Point3D readPoint3D(char* str, int& indx);

	mathem::Vector3D readNormal(char* str, int& indx);

	Graphics_Polygon readPolygon(char* str, int& indx, Mesh3D** md);

	/************************************************************************************
	Parse .obj
	-----------------------------------------------------------------------------------
	Application:
	Description:
	************************************************************************************/
	void parseMesh_OBJ(std::string filename, void** mesh, char* flag);

}

#endif