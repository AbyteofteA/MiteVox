
#ifndef RENDERER_API_H
#define RENDERER_API_H

#define HEXAGON_CONST 0.866025403

#include "engine/Math/src/Math.h"
#include "engine/FileIO/src/Formats/glTF_2.0/glTF.h"

#include "Cubemap.h"
#include "Color.h"
#include "Primitives.h"
#include "Skybox.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Material/Material.h"
#include "Camera.h"
#include "Mesh3D.h"
#include "Model3D.h"
#include "parseModel_OBJ.h"
#include "Light.h"
#include "Effects.h"
#include "RendererSettings.h"


namespace render
{

//==================================================================
//   RendererAPI interface declaration  |  ! under construction !  |
//==================================================================

	// General

	inline RendererSettings* initRenderer(int width, int height, bool isFullScreen, bool backfaceCulling, ColorRGBf clearColor);
	inline void closeRenderer(RendererSettings* renderer);
	inline std::string getVendorName();
	inline std::string getRendererName();
	inline std::string getVersion();
	inline std::string getLanguageVersion();

	// Shaders

	/*****************************************************************************************
	Creates and compiles a shader.
	\param name - shader name.
	\param filename - files path without extension.
					  Filenames must be the same for all types of shaders.
					  Expected extensions are: .vert.shader and .frag.shader
	*****************************************************************************************/
	inline int createShader(std::string name, std::string filename);
	inline void useShader(int shaderID);
	inline void deleteShader(int shaderID);

	// Buffers

	inline void clearBufferXY(ColorRGBf color);
	inline void clearBufferZ();
	inline void display(RendererSettings* renderer);

	// Primitives

	void drawPoint(RendererSettings* renderer, Point point);
	void drawLine(RendererSettings* renderer, Point point1, Point point2);
	void drawTriangle(RendererSettings* renderer, Point point1, Point point2, Point point3);

	void drawCross(RendererSettings* renderer, Point point, float size);
	void drawSnowflake(RendererSettings* renderer, Point point, float size);

	void renderPoints(RendererSettings* renderer, Camera* camera, mathem::Transform* cameraTransform);
	void renderLines(RendererSettings* renderer, Camera* camera, mathem::Transform* cameraTransform);
	void renderTriangles(RendererSettings* renderer, Camera* camera, mathem::Transform* cameraTransform);

	// Models

	void uploadModel3D(Model3D* model3D);
	void selectModel3D(Model3D* model3D);
	void removeModel3D(Model3D* model3D);
	void renderModel3D(RendererSettings* renderer, Model3D* model3D, mathem::Transform* transform, Camera* camera, mathem::Transform* cameraTransform);
	
	void uploadMaterial(fileio::Material* material, int shaderID);
	void selectMaterial(fileio::Material* material, int shaderID);
	void removeMaterial(fileio::Material* material, int shaderID);

	void uploadBufferView(fileio::BufferView* bufferView);
	void removeBufferView(fileio::BufferView* bufferView);

	void uploadMesh(fileio::Mesh* mesh, int shaderID);
	void removeMesh(fileio::Mesh* mesh);
	void renderMesh(RendererSettings* renderer, int shaderID, fileio::Mesh* mesh, mathem::Transform* transform, Camera* camera, mathem::Transform* cameraTransform);
	
	void renderWireframe(RendererSettings* renderer, Model3D* model3D, mathem::Transform* transform, ColorRGBAf color, Camera* camera, mathem::Transform* cameraTransform);

	inline void uploadSkybox(Skybox* skybox);
	inline void selectSkybox(Skybox* skybox);
	inline void removeSkybox(Skybox* skybox);
	inline void renderSkybox(RendererSettings* renderer, Skybox* skybox, Camera* camera, mathem::Transform* cameraTransform);
}

//==================================================================
//       Include your Renderer API implementation here             |
//==================================================================
#include "engine/Renderer/src/implementations/RendererOpenGL/RendererOpenGL.h"
//==================================================================

#endif