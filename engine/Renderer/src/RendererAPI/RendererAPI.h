#ifndef RENDERER_API_H
#define RENDERER_API_H

#define HEXAGON_CONST 0.866025403

#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/Renderer/src/RendererAPI/Primitives.h"
#include "engine/Renderer/src/RendererAPI/Cubemap.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"
#include "engine/Renderer/src/RendererAPI/Light.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/Math/src/Vector.h"
#include "engine/MiteVox/src/Material/Material.h"
#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/MiteVox/src/Skeleton/SkeletonBase.h"
#include "engine/MiteVox/src/Playground/Node.h"

namespace render
{

//==================================================================
//   RendererAPI interface declaration  |  ! under construction !  |
//==================================================================

	// General

	RendererSettings* initRenderer(int width, int height, bool isFullScreen, bool backfaceCulling, ColorRGBf clearColor);
	void closeRenderer(RendererSettings* renderer);
	std::string getVendorName();
	std::string getRendererName();
	std::string getVersion();
	std::string getLanguageVersion();
	void printErrors(const char* file, int line);
#define PRINT_RENDERER_ERRORS \
printErrors(__FILE__, __LINE__);

	// Shaders

	/*****************************************************************************************
	Creates and compiles a shader.
	\param name - shader name.
	\param filename - files path without extension.
					  Filenames must be the same for all types of shaders.
					  Expected extensions are: .vert.shader and .frag.shader
	*****************************************************************************************/
	int createShader(std::string name, std::string filename);
	void useShader(int shaderID);
	void deleteShader(int shaderID);

	// Buffers

	void createFramebuffer();
	void clearBufferXY(ColorRGBf color);
	void clearBufferZ();
	void display(RendererSettings* renderer);

	// Primitives

	void drawPoint(RendererSettings* renderer, Point point);
	void drawLine(RendererSettings* renderer, Point point1, Point point2);
	void drawTriangle(RendererSettings* renderer, Point point1, Point point2, Point point3);

	void drawCross(RendererSettings* renderer, Point point, float size);
	void drawArrow(RendererSettings* renderer, mathem::Vector3D origin, mathem::Vector3D direction, ColorRGBAf color);
	void drawSnowflake(RendererSettings* renderer, Point point, float size);

	void renderPoints(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform);
	void renderLines(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform);
	void renderTriangles(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform);

	// Models

	void setAmbientLight(mathem::Vector3D ambientLightColor, int shaderID);
	void uploadDirectionalLights(safety::SafeArray<render::DirectionalLight>* lightsArray, int shaderID);
	void uploadPointLights(safety::SafeArray<render::PointLight>* lightsArray, int shaderID);
	void uploadSpotLights(safety::SafeArray<render::SpotLight>* lightsArray, int shaderID);

	void uploadMaterial(mitevox::Material* material, int shaderID);
	void selectMaterial(mitevox::Material* material, int shaderID);
	void removeMaterial(mitevox::Material* material, int shaderID);

	void uploadBufferView(mitevox::BufferView* bufferView);
	void updateBufferView(mitevox::BufferView* bufferView);
	void removeBufferView(mitevox::BufferView* bufferView);

	mitevox::SkeletonBase* tryUploadSkeleton(mitevox::Node* node, int shaderID);

	void uploadMesh(mitevox::Mesh* mesh, int shaderID);
	void updateMesh(mitevox::Mesh* mesh, int shaderID);
	void removeMesh(mitevox::Mesh* mesh, int shaderID);
	void renderMesh(
		RendererSettings* renderer, 
		int shaderID, 
		mitevox::Mesh* mesh,
		mathem::GeometryTransform* transform,
		Camera* camera, 
		mathem::GeometryTransform* cameraTransform);

	void uploadSkybox(Cubemap* skybox, int shaderID);
	void selectSkybox(Cubemap* skybox, int shaderID);
	void removeSkybox(Cubemap* skybox, int shaderID);
	void renderSkybox(RendererSettings* renderer, int shaderID, Cubemap* skybox, Camera* camera, mathem::GeometryTransform* cameraTransform);
}

//==================================================================
//       Include your Renderer API implementation here             |
//==================================================================
#include "engine/Renderer/src/implementations/RendererOpenGL/RendererOpenGL.h"
//==================================================================

#endif