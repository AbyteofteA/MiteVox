
#ifndef RENDERER_API_H
#define RENDERER_API_H

#define HEXAGON_CONST 0.866025403

#include "Cubemap.h"
#include "Color.h"
#include "Primitives.h"
#include "Cubemap.h"
#include "engine/MiteVox/src/Material/Material.h"
#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/MiteVox/src/Skeleton/SkeletonBase.h"
#include "engine/MiteVox/src/Playground/Node.h"
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
	inline void printErrors(const char* file, int line);
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

	void renderPoints(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform);
	void renderLines(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform);
	void renderTriangles(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform);

	// Models

	void setAmbientLight(mathem::Vector3D ambientLightColor, int shaderID);
	void uploadPointLights(safety::SafeArray<render::PointLight>* lightsArray, int shaderID);
	void clearPointLights();

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

	// Nodes

	void renderNodeRecursively(
		RendererSettings* renderer,
		int shaderID,
		mitevox::Node* node,
		mathem::GeometryTransform* transform,
		Camera* camera,
		mathem::GeometryTransform* cameraTransform);

	void removeNodeRecursively(mitevox::Node* node, int shaderID);

	void renderWireframe(RendererSettings* renderer, Model3D* model3D, mathem::Transform* transform, ColorRGBAf color, Camera* camera, mathem::Transform* cameraTransform);

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