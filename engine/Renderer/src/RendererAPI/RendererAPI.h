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

#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/gtc/matrix_transform.hpp"
#include "dependencies/glm/gtc/type_ptr.hpp"

namespace render
{

//==================================================================
//   RendererAPI interface declaration  |  ! under construction !  |
//==================================================================

	// Constants

#define TEXTURE_UNIT_ALBEDO 0
#define TEXTURE_UNIT_METALLIC_ROUGHNESS 1
#define TEXTURE_UNIT_NORMAL 2
#define TEXTURE_UNIT_OCCLUSION 3
#define TEXTURE_UNIT_EMISSIVE 4
#define TEXTURE_UNIT_ENVIRONMENT 5

#define TEXTURE_UNIT_SPOT_SHADOWS_START 10 // [10; 25]

#define DEFAULT_ATTRIBUTE_POSITION 0
#define DEFAULT_ATTRIBUTE_NORMAL 1
#define DEFAULT_ATTRIBUTE_TEX_COORD_0 2
#define DEFAULT_ATTRIBUTE_TEX_COORD_1 3
#define DEFAULT_ATTRIBUTE_COLOR 4
#define DEFAULT_ATTRIBUTE_JOINTS 5
#define DEFAULT_ATTRIBUTE_WEIGHTS 6
#define DEFAULT_ATTRIBUTE_TANGENT 7

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

	void setWireframeRendering(bool isEnabled);
	void setViewport(int x, int y, int width, int height);
	void disableBlending();
	void setAdditiveBlending();
	void setAlphaBlending();

	// Shaders

	/*****************************************************************************************
	Creates and compiles a shader.
	\param name - shader name.
	\param filename - files path without extension.
					  Filenames must be the same for all types of shaders.
					  Expected extensions are: .vert.shader and .frag.shader
	*****************************************************************************************/
	int createShader(std::string name, std::string filename);
	int createComputeShader(std::string name, std::string filename);
	void useShader(int shaderID);
	void deleteShader(int shaderID);

	// Buffers

	int printFramebufferStatus();
	void renderScreenQuad();

	void activateDefaultFramebuffer(RendererSettings* renderer);
	void clearBufferXY(ColorRGBf color = ColorRGBf::BLACK());
	void clearBufferZ();
	void display(RendererSettings* renderer);

	void createGbuffer(RendererSettings* renderer);
	void activateGbuffer(RendererSettings* renderer);
	void deleteGbuffer();
	void createMainCanvas(RendererSettings* renderer);
	void activateMainCanvas(RendererSettings* renderer);
	void renderSceneFromGbuffer(
		RendererSettings* renderer,
		int shaderID,
		Camera* camera,
		mathem::GeometryTransform* cameraTransform,
		float deltaTime);
	void copyDepthFromGbufferToMainCanvas(RendererSettings* renderer);
	void renderSceneFromMainCanvas(RendererSettings* renderer, int shaderID, float deltaTime);
	void deleteMainCanvas();

	// Primitives

	void drawPoint(RendererSettings* renderer, Point point);
	void drawPoint(RendererSettings* renderer, mathem::Vector3D position, ColorRGBAf color);
	void drawLine(RendererSettings* renderer, Point point1, Point point2);
	void drawTriangle(RendererSettings* renderer, Point point1, Point point2, Point point3);

	void drawCross(RendererSettings* renderer, Point point, float size);
	void drawCross(RendererSettings* renderer, mathem::Vector3D position, ColorRGBAf color, float size);
	void drawArrow(RendererSettings* renderer, mathem::Vector3D origin, mathem::Vector3D direction, ColorRGBAf color);
	void drawSnowflake(RendererSettings* renderer, Point point, float size);
	void drawSnowflake(RendererSettings* renderer, mathem::Vector3D position, ColorRGBAf color, float size);

	void renderPoints(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform);
	void renderLines(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform);
	void renderTriangles(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform);

	// Models

	size_t getScreenQuadID();
	extern const float screenQuad[];
	size_t getUnitCubeID();
	extern const float unitCubePositions[];

	void resetLights(int shaderID);
	void setAmbientLight(mathem::Vector3D ambientLightColor, int shaderID);
	void uploadDirectionalLights(safety::SafeArray<render::DirectionalLight>* lightsArray, size_t offset, size_t count, int shaderID);
	void uploadPointLights(safety::SafeArray<render::PointLight>* lightsArray, size_t offset, size_t count, int shaderID);
	void uploadSpotLights(safety::SafeArray<render::SpotLight>* lightsArray, size_t offset, size_t count, int shaderID);

	void uploadMaterial(mitevox::Material* material);
	void selectMaterial(mitevox::Material* material, int shaderID);
	void removeMaterial(mitevox::Material* material);

	void uploadBufferView(mitevox::BufferView* bufferView);
	void updateBufferView(mitevox::BufferView* bufferView);
	void removeBufferView(mitevox::BufferView* bufferView);

	mitevox::SkeletonBase* tryUploadSkeleton(mitevox::Node* node, int shaderID);

	void uploadMesh(mitevox::Mesh* mesh);
	void updateMesh(mitevox::Mesh* mesh);
	void removeMesh(mitevox::Mesh* mesh);
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
	void renderSkybox(
		RendererSettings* renderer, 
		int shaderID, 
		Cubemap* skybox, 
		Camera* camera, 
		mathem::GeometryTransform* cameraTransform);

	// Shadows

	void renderMeshToShadowMap(
		RendererSettings* renderer,
		int shaderID,
		mitevox::Mesh* mesh,
		mathem::GeometryTransform* transform);
}

//==================================================================
//       Include your Renderer API implementation here             |
//==================================================================
#include "engine/Renderer/src/implementations/RendererOpenGL/RendererOpenGL.h"
//==================================================================

#endif