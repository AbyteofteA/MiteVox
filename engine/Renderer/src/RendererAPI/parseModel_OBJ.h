
#ifndef PARSEMODEL_OBJ_H
#define PARSEMODEL_OBJ_H

#include <regex>
#include "FileIO/src/FileIO.h"
#include "Math/src/Math.h"

#include "ColorRGBA.h"
#include "Mesh3D.h"

namespace render
{

	inline double* readFloats3(char* str, int& indx)
	{
		double resultFloats[3] = { 0 };

		char* indxTmp = nullptr;
		char* tmpStr = nullptr;

		for (int i = 0; i < 3; i++)
		{
			while (str[indx] == ' ')
				indx++;
			if ((str[indx] == '\n') || (str[indx] == '\0'))
				break;

			indxTmp = str + indx;	// Save the initial index
			tmpStr = nullptr;
			resultFloats[i] = strtod(str + indx, &tmpStr);

			indx += (int)(tmpStr - indxTmp);
		}

		return resultFloats;
	}


	inline mathem::Point3D readPoint3D(char* str, int& indx)
	{
		mathem::Point3D v;
		double* tmpFloats = readFloats3(str, indx);
		v.x = (float)tmpFloats[0];
		v.y = (float)tmpFloats[1];
		v.z = (float)tmpFloats[2];

		return v;
	}

	inline mathem::Vector3D readNormal(char* str, int& indx)
	{
		mathem::Vector3D v;
		double* tmpFloats = readFloats3(str, indx);
		v.i = (float)tmpFloats[0];
		v.j = (float)tmpFloats[1];
		v.k = (float)tmpFloats[2];

		return v;
	}



	inline Polygon readPolygon(char* str, int& indx, Mesh3D** md)
	{
		Polygon poly;

		//============== Allocating memory for the string after 'f' ==============

		char* tmpStr = nullptr; int size = 1;
		while (str[indx] != '\n' && (str[indx] != '\0'))
		{
			size++; indx++;
		}
		tmpStr = (char*)realloc(tmpStr, sizeof(char) * size);
		indx -= (size - 1);

		//======================= Assigning the values ===========================

		int i = 0;
		while (i < (size - 1))
		{
			tmpStr[i] = str[indx];
			i++; indx++;
		}
		tmpStr[size - 1] = '\0';

		//============================= Parsing ==================================

		i = 0;
		char* endPtr = tmpStr;

		/*while(tmpStr[i] == ' ')
			i++;*/

		while (endPtr[0] != '\0')
		{
			long a = strtol(endPtr + i, &endPtr, 0);
			if (a < 0)
				a = (*md)->amOfVertices + a + 1;
			poly.amOfVertices += 1;
			poly.p = (long*)realloc(poly.p, sizeof(long) * poly.amOfVertices);
			poly.p[poly.amOfVertices - 1] = a;

			if (endPtr[0] == '\\' || endPtr[0] == '/')
				endPtr = endPtr + 1;
			else if (endPtr[0] == ' ')
			{
				endPtr = endPtr + 1;
				continue;
			}
			else if (endPtr[0] == '\0')
				break;


			if (endPtr[0] != '/')
			{
				a = strtol(endPtr + i, &endPtr, 0);
				if (a < 0)
					a = (*md)->amOfTextureCoords + a + 1;
				poly.t = (long*)realloc(poly.t, sizeof(long) * poly.amOfVertices);
				poly.t[poly.amOfVertices - 1] = a;

				if (endPtr[0] == '\\' || endPtr[0] == '/')
					endPtr = endPtr + 1;
				else if (endPtr[0] == ' ')
				{
					endPtr = endPtr + 1;
					continue;
				}
				else if (endPtr[0] == '\0')
					break;
			}
			else endPtr = endPtr + 1;


			a = strtol(endPtr + i, &endPtr, 0);
			if (a < 0)
				a = (*md)->amOfNormals + a + 1;
			poly.n = (long*)realloc(poly.n, sizeof(long) * poly.amOfVertices);
			poly.n[poly.amOfVertices - 1] = a;

			if (endPtr[0] == '\\' || endPtr[0] == '/' || endPtr[0] == ' ')
			{
				endPtr = endPtr + 1;
				continue;
			}
			else if (endPtr[0] == '\0')
				break;
		}

		return poly;
	}


	/************************************************************************************
	Parse .obj
	-----------------------------------------------------------------------------------
	Application:
	Description:
	************************************************************************************/
	inline void parseModel_OBJ(std::string filename, void** mesh, fileio::FileStatus* flag)
	{
		*mesh = nullptr;
		*flag = fileio::FileStatus::LOADING;
		char* fileData = nullptr;

		fileio::FileStatus tmpFlag = fileio::FileStatus::LOADING;
		fileio::loadBytes(filename, (void**)&fileData, &tmpFlag);

		auto const regex = std::regex(fileio::OBJ_regex);
		bool const myTextContainsRegex = std::regex_search(std::string(fileData), regex);
		if (myTextContainsRegex)
		{
			//printf("\nOBJ is correct.\n");
		}
		else
		{
			//printf("\nOBJ error.\n");
		}
		
		Mesh3D* meshTmp = new Mesh3D();

		bool hasNormals = false;

		meshTmp->type = 0;

		meshTmp->amOfVertices = 0;
		meshTmp->v = nullptr;

		meshTmp->vMax.x = 0;
		meshTmp->vMax.y = 0;
		meshTmp->vMax.z = 0;

		meshTmp->vMin.x = 0;
		meshTmp->vMin.y = 0;
		meshTmp->vMin.z = 0;

		meshTmp->amOfTextureCoords = 0;
		meshTmp->vt = nullptr;

		meshTmp->amOfNormals = 0;
		meshTmp->vn = nullptr;

		meshTmp->amOfFaces = 0;
		meshTmp->f = nullptr;


		//=========================== Looking for normals ============================

		int amOfVrtcs = 0;
		int amOfFaces = 0;

		int i = 0;
		while (!hasNormals && (fileData[i] != '\0'))
		{
			while ((i > -1) && (fileData[i] != '\n') && (fileData[i] != '\0'))
			{
				switch (fileData[i])
				{
				case '#':
					while (fileData[i] != '\n')
						i++;
					break;

				case 'v':
					if (fileData[i + 1] == ' ')
					{
						amOfVrtcs++;
						i += 2;
						break;
					}
					else if (fileData[i + 1] == 't')
						break;
					else if (fileData[i + 1] == 'n')
					{
						hasNormals = true;
						break;
					}
					break;

				case 'f':
				{
					amOfFaces++;
					i += 2;
					break;
				}
				default:
					break;
				}
				i++;
			}
			i++;
		}

		if (!hasNormals)
		{
			meshTmp->vn = (mathem::Vector3D*)realloc(meshTmp->vn, sizeof(mathem::Vector3D) * amOfVrtcs);
			mathem::Vector3D v = { 0, 0, 0, 0 };

			for (int i = 0; i < amOfVrtcs; i++)
				meshTmp->vn[i] = v;
			meshTmp->amOfNormals = amOfVrtcs;
		}


		//================================ Parsing ==================================

		i = 0;
		while (fileData[i] != '\0')
		{
			while (fileData[i] != '\n' && (fileData[i] != '\0'))
			{
				switch (fileData[i])
				{

				case '#':
					while (fileData[i] != '\n')
						i++;
					break;

				case 'v':
					if (fileData[i + 1] == ' ')
					{
						i += 2;
						meshTmp->amOfVertices += 1;
						meshTmp->v = (mathem::Point3D*)realloc(meshTmp->v, sizeof(mathem::Point3D) * meshTmp->amOfVertices);
						meshTmp->v[meshTmp->amOfVertices - 1] = readPoint3D(fileData, i);

						if (meshTmp->vMax.x < meshTmp->v[meshTmp->amOfVertices - 1].x)
							meshTmp->vMax.x = meshTmp->v[meshTmp->amOfVertices - 1].x;
						if (meshTmp->vMax.y < meshTmp->v[meshTmp->amOfVertices - 1].y)
							meshTmp->vMax.y = meshTmp->v[meshTmp->amOfVertices - 1].y;
						if (meshTmp->vMax.z < meshTmp->v[meshTmp->amOfVertices - 1].z)
							meshTmp->vMax.z = meshTmp->v[meshTmp->amOfVertices - 1].z;

						if (meshTmp->vMin.x > meshTmp->v[meshTmp->amOfVertices - 1].x)
							meshTmp->vMin.x = meshTmp->v[meshTmp->amOfVertices - 1].x;
						if (meshTmp->vMin.y > meshTmp->v[meshTmp->amOfVertices - 1].y)
							meshTmp->vMin.y = meshTmp->v[meshTmp->amOfVertices - 1].y;
						if (meshTmp->vMin.z > meshTmp->v[meshTmp->amOfVertices - 1].z)
							meshTmp->vMin.z = meshTmp->v[meshTmp->amOfVertices - 1].z;

						break;
					}
					else if (fileData[i + 1] == 'n')
					{
						i += 1;
						if (fileData[i + 1] == ' ')
						{
							i += 2;
							meshTmp->amOfNormals += 1;
							meshTmp->vn = (mathem::Vector3D*)realloc(meshTmp->vn, sizeof(mathem::Vector3D) * meshTmp->amOfNormals);
							meshTmp->vn[meshTmp->amOfNormals - 1] = readNormal(fileData, i);
						}
						break;
					}
					else if (fileData[i + 1] == 't')
					{
						i += 1;
						if (fileData[i + 1] == ' ')
						{
							i += 2;
							meshTmp->amOfTextureCoords += 1;
							meshTmp->vt = (mathem::Point3D*)realloc(meshTmp->vt, sizeof(mathem::Point3D) * meshTmp->amOfTextureCoords);
							meshTmp->vt[meshTmp->amOfTextureCoords - 1] = readPoint3D(fileData, i);
						}
						break;
					}
					break;

				case 'f':
					if (fileData[i + 1] == ' ')
					{
						i += 2;
						meshTmp->amOfFaces += 1;
						meshTmp->f = (Polygon*)realloc(meshTmp->f, sizeof(Polygon) * meshTmp->amOfFaces);
						Polygon poligon = readPolygon(fileData, i, &meshTmp);

						if (!hasNormals)
						{
							mathem::Vector3D vectA = { meshTmp->v[poligon.p[2] - 1].x - meshTmp->v[poligon.p[1] - 1].x,
											meshTmp->v[poligon.p[2] - 1].y - meshTmp->v[poligon.p[1] - 1].y,
											meshTmp->v[poligon.p[2] - 1].z - meshTmp->v[poligon.p[1] - 1].z };

							mathem::Vector3D vectB = { meshTmp->v[poligon.p[0] - 1].x - meshTmp->v[poligon.p[1] - 1].x,
											meshTmp->v[poligon.p[0] - 1].y - meshTmp->v[poligon.p[1] - 1].y,
											meshTmp->v[poligon.p[0] - 1].z - meshTmp->v[poligon.p[1] - 1].z };

							mathem::Vector3D vectN = { vectA.j * vectB.k - vectB.j * vectA.k,
											vectB.i * vectA.k - vectA.i * vectB.k,
											vectA.i * vectB.j - vectB.i * vectA.j };

							vectN.length = sqrt(pow(vectN.i, 2) + pow(vectN.j, 2) + pow(vectN.k, 2));


							poligon.n = (long*)malloc(sizeof(long) * poligon.amOfVertices);
							for (int j = 0; j < poligon.amOfVertices; j++)
							{
								poligon.n[j] = poligon.p[j];

								meshTmp->vn[poligon.n[j] - 1].i += vectN.i;
								meshTmp->vn[poligon.n[j] - 1].j += vectN.j;
								meshTmp->vn[poligon.n[j] - 1].k += vectN.k;

								meshTmp->vn[poligon.n[j] - 1].length = sqrt(
									pow(meshTmp->vn[poligon.n[j] - 1].i, 2) +
									pow(meshTmp->vn[poligon.n[j] - 1].j, 2) +
									pow(meshTmp->vn[poligon.n[j] - 1].k, 2));
							}
						}

						if (poligon.amOfVertices == 3)
						{
							meshTmp->f[meshTmp->amOfFaces - 1] = poligon;
							meshTmp->f[meshTmp->amOfFaces - 1].color = (ColorRGBA*)malloc(sizeof(ColorRGBA) * 3);
						}
						else
						{
							int kMax = poligon.amOfVertices;
							for (int k = 1; k < kMax - 1; k++)
							{
								Polygon pTmp = { nullptr, nullptr, nullptr, nullptr, 3 };

								if (meshTmp->v != nullptr)
								{
									pTmp.p = (long*)malloc(sizeof(long) * 3);
									pTmp.p[0] = poligon.p[0];
									pTmp.p[1] = poligon.p[k];
									pTmp.p[2] = poligon.p[k + 1];
								}
								if (meshTmp->vt != nullptr && poligon.t != nullptr)
								{
									pTmp.t = (long*)malloc(sizeof(long) * 3);
									pTmp.t[0] = poligon.t[0];
									pTmp.t[1] = poligon.t[k];
									pTmp.t[2] = poligon.t[k + 1];
								}
								if (meshTmp->vn != nullptr)
								{
									pTmp.n = (long*)malloc(sizeof(long) * 3);
									pTmp.n[0] = poligon.n[0];
									pTmp.n[1] = poligon.n[k];
									pTmp.n[2] = poligon.n[k + 1];
								}
								pTmp.color = (ColorRGBA*)malloc(sizeof(ColorRGBA) * 3);

								if (k != 1)
								{
									meshTmp->amOfFaces += 1;
									meshTmp->f = (Polygon*)realloc(meshTmp->f, sizeof(Polygon) * meshTmp->amOfFaces);
									meshTmp->f[meshTmp->amOfFaces - 1] = pTmp;
								}
								else
									meshTmp->f[meshTmp->amOfFaces - 1] = pTmp;
							}
						}

						break;
					}
					break;

				default:
					break;
				}

				i++;

				//printf("\namOfVertices == %d\n", entity.amOfVertices);
				//printf("i == %d\n", i);
			}
			i++;
		}

		unsigned char r, g, b;
		for (long i = 0; i < meshTmp->amOfFaces; i++)
		{
			r = (unsigned char)abs((meshTmp->v[meshTmp->f[i].p[0] - 1].x - meshTmp->vMin.x) / (meshTmp->vMax.x - meshTmp->vMin.x)) * 255;
			g = (unsigned char)abs((meshTmp->v[meshTmp->f[i].p[0] - 1].y - meshTmp->vMin.y) / (meshTmp->vMax.y - meshTmp->vMin.y)) * 255;
			b = (unsigned char)abs((meshTmp->v[meshTmp->f[i].p[0] - 1].z - meshTmp->vMin.z) / (meshTmp->vMax.z - meshTmp->vMin.z)) * 255;

			meshTmp->f[i].color[0] = { r, g, b };

			r = (unsigned char)abs((meshTmp->v[meshTmp->f[i].p[1] - 1].x - meshTmp->vMin.x) / (meshTmp->vMax.x - meshTmp->vMin.x)) * 255;
			g = (unsigned char)abs((meshTmp->v[meshTmp->f[i].p[1] - 1].y - meshTmp->vMin.y) / (meshTmp->vMax.y - meshTmp->vMin.y)) * 255;
			b = (unsigned char)abs((meshTmp->v[meshTmp->f[i].p[1] - 1].z - meshTmp->vMin.z) / (meshTmp->vMax.z - meshTmp->vMin.z)) * 255;

			meshTmp->f[i].color[1] = { r, g, b };

			r = (unsigned char)abs((meshTmp->v[meshTmp->f[i].p[2] - 1].x - meshTmp->vMin.x) / (meshTmp->vMax.x - meshTmp->vMin.x)) * 255;
			g = (unsigned char)abs((meshTmp->v[meshTmp->f[i].p[2] - 1].y - meshTmp->vMin.y) / (meshTmp->vMax.y - meshTmp->vMin.y)) * 255;
			b = (unsigned char)abs((meshTmp->v[meshTmp->f[i].p[2] - 1].z - meshTmp->vMin.z) / (meshTmp->vMax.z - meshTmp->vMin.z)) * 255;

			meshTmp->f[i].color[2] = { r, g, b };

			for (int j = 0; j < meshTmp->f[i].amOfVertices; j++)
			{
				//meshTmp->f[i].n[j] = meshTmp->f[i].p[j];
				meshTmp->vn[meshTmp->f[i].n[j] - 1].length = sqrt(
					pow(meshTmp->vn[meshTmp->f[i].n[j] - 1].i, 2) +
					pow(meshTmp->vn[meshTmp->f[i].n[j] - 1].j, 2) +
					pow(meshTmp->vn[meshTmp->f[i].n[j] - 1].k, 2));
			}
		}

		free(fileData);
		(*mesh) = (void*)meshTmp;

		*flag = fileio::FileStatus::READY;
		return;
	}

}


#endif
