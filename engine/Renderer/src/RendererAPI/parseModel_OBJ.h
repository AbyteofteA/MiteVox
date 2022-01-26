
#ifndef PARSEMODEL_OBJ_H
#define PARSEMODEL_OBJ_H

#include <atomic>
#include <string>
#include <regex>
#include "engine/FileIO/src/FileInputOutput.h"
#include "engine/FileIO/src/Formats/WavefrontOBJ/WavefrontOBJRegex.h"
#include "engine/FileIO/src/Formats/WavefrontOBJ/WavefrontOBJRegexPack.h"
#include "engine/Math/src/Math.h"

#include "Color.h"
#include "Primitives.h"
#include "Mesh3D.h"

namespace render
{
	inline bool isNotEndOfLine(char currentChar)
	{
		if (currentChar >= 32)
		{
			return true;
		}
		return false;
	}

	inline std::string extractLine(char* str, int& indx)
	{
		int size = 0;
		while (isNotEndOfLine(str[indx + size]))
		{
			++size;
		}
		std::string line(str + indx, size);
		indx += size;
		return line;
	}

	inline double* readFloats3(char* str, int& indx)
	{
		double resultFloats[3] = { 0 };

		char* indxTmp = nullptr;
		char* tmpStr = nullptr;

		for (int i = 0; i < 3; i++)
		{
			while (str[indx] == ' ')
				indx++;
			if (isNotEndOfLine(str[indx]) == false)
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

	inline Polygon readPolygon(char* str, int& indx, Mesh3D** md, fileio::WavefrontOBJRegexPack* regexPack)
	{
		Polygon polygon;

		indx -= 2;
		std::string polygonStr = extractLine(str, indx);

		std::smatch matchPolygonSimple;
		std::smatch matchPolygonWithNormals;
		std::smatch matchPolygonWirhTextures;
		std::smatch matchPolygonComplete;
		bool polygonIsSimple = std::regex_search(polygonStr, matchPolygonSimple, regexPack->polygonSimple);
		bool polygonIsWithNormals = std::regex_search(polygonStr, matchPolygonWithNormals, regexPack->polygonWithTexture);
		bool polygonIsWirhTextures = std::regex_search(polygonStr, matchPolygonWirhTextures, regexPack->polygonWithNormals);
		bool polygonIsComplete = std::regex_search(polygonStr, matchPolygonComplete, regexPack->polygonComplete);

		long positionIndex = 0;
		long normalIndex = 0;
		long textureCoordIndex = 0;

		// TODO: implement polygon decomposition 

		if (polygonIsSimple)
		{
			polygon.setType(PolygonType::POSITIONS);
			polygonStr = matchPolygonSimple[1].str();
			std::sregex_iterator regexIterator(polygonStr.begin(), polygonStr.end(), regexPack->intNumber);
			std::sregex_iterator end;
			size_t index = 0;
			while (regexIterator != end)
			{
				positionIndex = std::stol((*regexIterator)[1].str());
				if (positionIndex < 0)
					positionIndex = (*md)->positionsCount + positionIndex + 1;
				polygon.setVertexSimple(index, positionIndex);
				++regexIterator;
				++index;
			}
		}
		else if (polygonIsWithNormals)
		{
			polygon.setType(PolygonType::POSITIONS_AND_NORMALS);
			polygonStr = matchPolygonWithNormals[1].str();
			std::sregex_iterator regexIterator(polygonStr.begin(), polygonStr.end(), regexPack->vertexWithNormal);
			std::sregex_iterator end;
			size_t index = 0;
			while (regexIterator != end)
			{
				positionIndex = std::stol((*regexIterator)[1].str());
				if (positionIndex < 0)
					positionIndex = (*md)->positionsCount + positionIndex + 1;
				normalIndex = std::stol((*regexIterator)[2].str());
				if (normalIndex < 0)
					normalIndex = (*md)->normalsCount + normalIndex + 1;
				polygon.setVertexWithNormal(index, positionIndex, normalIndex);
				++regexIterator;
				++index;
			}
		}
		else if (polygonIsWirhTextures)
		{
			polygon.setType(PolygonType::POSITIONS_AND_TEXTURE);
			polygonStr = matchPolygonWirhTextures[1].str();
			std::sregex_iterator regexIterator(polygonStr.begin(), polygonStr.end(), regexPack->vertexComplete);
			std::sregex_iterator end;
			size_t index = 0;
			while (regexIterator != end)
			{
				positionIndex = std::stol((*regexIterator)[1].str());
				if (positionIndex < 0)
					positionIndex = (*md)->positionsCount + positionIndex + 1;
				textureCoordIndex = std::stol((*regexIterator)[2].str());
				if (textureCoordIndex < 0)
					textureCoordIndex = (*md)->textureCoordsCount + textureCoordIndex + 1;
				polygon.setVertexWithTexture(index, positionIndex, textureCoordIndex);
				++regexIterator;
				++index;
			}
		}
		else if (polygonIsComplete)
		{
			polygon.setType(PolygonType::COMPLETE);
			polygonStr = matchPolygonComplete[1].str();
			std::sregex_iterator regexIterator(polygonStr.begin(), polygonStr.end(), regexPack->vertexComplete);
			std::sregex_iterator end;
			size_t index = 0;
			while (regexIterator != end)
			{
				positionIndex = std::stol((*regexIterator)[1].str());
				if (positionIndex < 0)
					positionIndex = (*md)->positionsCount + positionIndex + 1;
				textureCoordIndex = std::stol((*regexIterator)[2].str());
				if (textureCoordIndex < 0)
					textureCoordIndex = (*md)->textureCoordsCount + textureCoordIndex + 1;
				normalIndex = std::stol((*regexIterator)[3].str());
				if (normalIndex < 0)
					normalIndex = (*md)->normalsCount + normalIndex + 1;
				polygon.setVertexComplete(index, positionIndex, normalIndex, textureCoordIndex);
				++regexIterator;
				++index;
			}
		}

		return polygon;
	}

	/************************************************************************************
	Parse .obj
	-----------------------------------------------------------------------------------
	Application:
	Description:
	************************************************************************************/
	inline void parseModel_OBJ(std::string filename, void** mesh, std::atomic<fileio::FileStatus>* flag)
	{
		fileio::WavefrontOBJRegexPack regexPack;

		*mesh = nullptr;
		flag->store(fileio::FileStatus::LOADING);
		char* fileData = nullptr;

		std::atomic<fileio::FileStatus> tmpFlag = fileio::FileStatus::LOADING;
		fileio::FileInputOutput::loadBinary(filename, (void**)&fileData, &tmpFlag);
		
		Mesh3D* meshTmp = new Mesh3D();

		bool hasNormals = false;

		meshTmp->type = 0;

		meshTmp->positionsCount = 0;
		meshTmp->positions = nullptr;

		meshTmp->minPosition.x = 0;
		meshTmp->minPosition.y = 0;
		meshTmp->minPosition.z = 0;

		meshTmp->maxPosition.x = 0;
		meshTmp->maxPosition.y = 0;
		meshTmp->maxPosition.z = 0;

		meshTmp->textureCoordsCount = 0;
		meshTmp->textureCoords = nullptr;

		meshTmp->normalsCount = 0;
		meshTmp->normals = nullptr;

		meshTmp->polygonsCount = 0;
		meshTmp->polygons = nullptr;


		//=========================== Looking for normals ============================

		int amOfVrtcs = 0;
		int amOfFaces = 0;

		int i = 0;
		while ((i > -1) && !hasNormals && fileData[i] != '\0')
		{
			switch (fileData[i])
			{
			case '#':
				while (isNotEndOfLine(fileData[i]))
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

		if (hasNormals == false)
		{
			meshTmp->normals = (mathem::Vector3D*)realloc(meshTmp->normals, sizeof(mathem::Vector3D) * amOfVrtcs);
			mathem::Vector3D v = { 0, 0, 0 };

			for (int i = 0; i < amOfVrtcs; i++)
				meshTmp->normals[i] = v;
			meshTmp->normalsCount = amOfVrtcs;
		}


		//================================ Parsing ==================================

		i = 0;
		while (i > -1 && fileData[i] != '\0')
		{
			while (isNotEndOfLine(fileData[i]))
			{
				switch (fileData[i])
				{

				case '#':
					while (isNotEndOfLine(fileData[i]))
						i++;
					break;

				case 'v':
					if (fileData[i + 1] == ' ')
					{
						i += 2;
						meshTmp->positionsCount += 1;
						meshTmp->positions = (mathem::Point3D*)realloc(meshTmp->positions, sizeof(mathem::Point3D) * meshTmp->positionsCount);
						meshTmp->positions[meshTmp->positionsCount - 1] = readPoint3D(fileData, i);

						if (meshTmp->minPosition.x > meshTmp->positions[meshTmp->positionsCount - 1].x)
							meshTmp->minPosition.x = meshTmp->positions[meshTmp->positionsCount - 1].x;
						if (meshTmp->minPosition.y > meshTmp->positions[meshTmp->positionsCount - 1].y)
							meshTmp->minPosition.y = meshTmp->positions[meshTmp->positionsCount - 1].y;
						if (meshTmp->minPosition.z > meshTmp->positions[meshTmp->positionsCount - 1].z)
							meshTmp->minPosition.z = meshTmp->positions[meshTmp->positionsCount - 1].z;

						if (meshTmp->maxPosition.x < meshTmp->positions[meshTmp->positionsCount - 1].x)
							meshTmp->maxPosition.x = meshTmp->positions[meshTmp->positionsCount - 1].x;
						if (meshTmp->maxPosition.y < meshTmp->positions[meshTmp->positionsCount - 1].y)
							meshTmp->maxPosition.y = meshTmp->positions[meshTmp->positionsCount - 1].y;
						if (meshTmp->maxPosition.z < meshTmp->positions[meshTmp->positionsCount - 1].z)
							meshTmp->maxPosition.z = meshTmp->positions[meshTmp->positionsCount - 1].z;

						break;
					}
					else if (fileData[i + 1] == 'n')
					{
						i += 1;
						if (fileData[i + 1] == ' ')
						{
							i += 2;
							meshTmp->normalsCount += 1;
							meshTmp->normals = (mathem::Vector3D*)realloc(meshTmp->normals, sizeof(mathem::Vector3D) * meshTmp->normalsCount);
							meshTmp->normals[meshTmp->normalsCount - 1] = readNormal(fileData, i);
						}
						break;
					}
					else if (fileData[i + 1] == 't')
					{
						i += 1;
						if (fileData[i + 1] == ' ')
						{
							i += 2;
							meshTmp->textureCoordsCount += 1;
							meshTmp->textureCoords = (mathem::Point3D*)realloc(meshTmp->textureCoords, sizeof(mathem::Point3D) * meshTmp->textureCoordsCount);
							meshTmp->textureCoords[meshTmp->textureCoordsCount - 1] = readPoint3D(fileData, i);
						}
						break;
					}
					break;

				case 'f':
					if (fileData[i + 1] == ' ')
					{
						i += 2;
						meshTmp->polygonsCount += 1;
						meshTmp->polygons = (Polygon*)realloc(meshTmp->polygons, sizeof(Polygon) * meshTmp->polygonsCount);
						Polygon poligon = readPolygon(fileData, i, &meshTmp, &regexPack);

						if (hasNormals == false)
						{
							mathem::Vector3D vectA = { meshTmp->positions[poligon.positions[2] - 1].x - meshTmp->positions[poligon.positions[1] - 1].x,
											meshTmp->positions[poligon.positions[2] - 1].y - meshTmp->positions[poligon.positions[1] - 1].y,
											meshTmp->positions[poligon.positions[2] - 1].z - meshTmp->positions[poligon.positions[1] - 1].z };

							mathem::Vector3D vectB = { meshTmp->positions[poligon.positions[0] - 1].x - meshTmp->positions[poligon.positions[1] - 1].x,
											meshTmp->positions[poligon.positions[0] - 1].y - meshTmp->positions[poligon.positions[1] - 1].y,
											meshTmp->positions[poligon.positions[0] - 1].z - meshTmp->positions[poligon.positions[1] - 1].z };

							mathem::Vector3D vectN = { vectA.j * vectB.k - vectB.j * vectA.k,
											vectB.i * vectA.k - vectA.i * vectB.k,
											vectA.i * vectB.j - vectB.i * vectA.j };

							for (size_t j = 0; j < 3; j++)
							{
								poligon.normals[j] = poligon.positions[j];

								meshTmp->normals[poligon.normals[j] - 1].i += vectN.i;
								meshTmp->normals[poligon.normals[j] - 1].j += vectN.j;
								meshTmp->normals[poligon.normals[j] - 1].k += vectN.k;
							}
						}

						if (true /* :) */)
						{
							meshTmp->polygons[meshTmp->polygonsCount - 1] = poligon;
						}
						/*else
						{
							int kMax = poligon.amOfVertices;
							for (int k = 1; k < kMax - 1; k++)
							{
								Polygon pTmp = { nullptr, nullptr, nullptr, nullptr, 3 };

								if (meshTmp->positions != nullptr)
								{
									pTmp.positions = (long*)malloc(sizeof(long) * 3);
									pTmp.positions[0] = poligon.positions[0];
									pTmp.positions[1] = poligon.positions[k];
									pTmp.positions[2] = poligon.positions[k + 1];
								}
								if (meshTmp->textureCoords != nullptr && poligon.textureCoords != nullptr)
								{
									pTmp.textureCoords = (long*)malloc(sizeof(long) * 3);
									pTmp.textureCoords[0] = poligon.textureCoords[0];
									pTmp.textureCoords[1] = poligon.textureCoords[k];
									pTmp.textureCoords[2] = poligon.textureCoords[k + 1];
								}
								if (meshTmp->normals != nullptr)
								{
									pTmp.normals = (long*)malloc(sizeof(long) * 3);
									pTmp.normals[0] = poligon.normals[0];
									pTmp.normals[1] = poligon.normals[k];
									pTmp.normals[2] = poligon.normals[k + 1];
								}
								pTmp.colors = (ColorRGBA*)malloc(sizeof(ColorRGBA) * 3);

								if (k != 1)
								{
									meshTmp->polygonsCount += 1;
									meshTmp->polygons = (Polygon*)realloc(meshTmp->polygons, sizeof(Polygon) * meshTmp->polygonsCount);
									meshTmp->polygons[meshTmp->polygonsCount - 1] = pTmp;
								}
								else
									meshTmp->polygons[meshTmp->polygonsCount - 1] = pTmp;
							}
						}*/

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
		for (size_t i = 0; i < meshTmp->polygonsCount; i++)
		{
			r = (unsigned char)abs((meshTmp->positions[meshTmp->polygons[i].positions[0] - 1].x - meshTmp->minPosition.x) /
				(meshTmp->maxPosition.x - meshTmp->minPosition.x)) * 255;
			g = (unsigned char)abs((meshTmp->positions[meshTmp->polygons[i].positions[0] - 1].y - meshTmp->minPosition.y) /
				(meshTmp->maxPosition.y - meshTmp->minPosition.y)) * 255;
			b = (unsigned char)abs((meshTmp->positions[meshTmp->polygons[i].positions[0] - 1].z - meshTmp->minPosition.z) /
				(meshTmp->maxPosition.z - meshTmp->minPosition.z)) * 255;

			meshTmp->polygons[i].colors[0] = { r, g, b };

			r = (unsigned char)abs((meshTmp->positions[meshTmp->polygons[i].positions[1] - 1].x - meshTmp->minPosition.x) /
				(meshTmp->maxPosition.x - meshTmp->minPosition.x)) * 255;
			g = (unsigned char)abs((meshTmp->positions[meshTmp->polygons[i].positions[1] - 1].y - meshTmp->minPosition.y) /
				(meshTmp->maxPosition.y - meshTmp->minPosition.y)) * 255;
			b = (unsigned char)abs((meshTmp->positions[meshTmp->polygons[i].positions[1] - 1].z - meshTmp->minPosition.z) /
				(meshTmp->maxPosition.z - meshTmp->minPosition.z)) * 255;

			meshTmp->polygons[i].colors[1] = { r, g, b };

			r = (unsigned char)abs((meshTmp->positions[meshTmp->polygons[i].positions[2] - 1].x - meshTmp->minPosition.x) /
				(meshTmp->maxPosition.x - meshTmp->minPosition.x)) * 255;
			g = (unsigned char)abs((meshTmp->positions[meshTmp->polygons[i].positions[2] - 1].y - meshTmp->minPosition.y) /
				(meshTmp->maxPosition.y - meshTmp->minPosition.y)) * 255;
			b = (unsigned char)abs((meshTmp->positions[meshTmp->polygons[i].positions[2] - 1].z - meshTmp->minPosition.z) /
				(meshTmp->maxPosition.z - meshTmp->minPosition.z)) * 255;

			meshTmp->polygons[i].colors[2] = { r, g, b };
		}

		free(fileData);
		(*mesh) = (void*)meshTmp;

		flag->store(fileio::FileStatus::READY);
		return;
	}

}


#endif
