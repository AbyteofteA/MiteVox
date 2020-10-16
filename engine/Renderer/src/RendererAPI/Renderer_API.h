
#ifndef RENDERER_API_H
#define RENDERER_API_H

#define HEXAGON_CONST 0.866025403

#include "Math/src/Math_Headers.h"
#include "FileIO/src/FileIO_Headers.h"

#include "Renderer_Cubemap.h"
#include "Renderer_ColorRGBA.h"
#include "Renderer_Primitives.h"
#include "Renderer_Skybox.h"
#include "Renderer_Material.h"
#include "Renderer_Camera.h"
#include "Renderer_Mesh3D.h"
#include "Renderer_Model3D.h"
#include "Renderer_Light.h"
#include "Effects.h"
#include "Renderer_Settings.h"


namespace render
{

//==================================================================
//   RendererAPI interface declaration  |  ! under construction !  |
//==================================================================

	// <General>

	inline int initRenderer();
	inline void clearBufferXY(float R, float G, float B);
	inline void clearBufferZ();
	inline void display();

	// Primitives

	void drawPoint(Point point);
	void drawLine(Point point1, Point point2);
	void drawTriangle(Point point1, Point point2, Point point3);

	void drawCross(Point point, float size);
	void drawSnowflake(Point point, float size);

	void renderPoints(Camera* camera, mathem::Transform* cameraTransform);
	void renderLines(Camera* camera, mathem::Transform* cameraTransform);
	void renderTriangles(Camera* camera, mathem::Transform* cameraTransform);

	// Models

	void uploadModel3D(Model3D* model3D);
	void removeModel3D(Model3D* model3D);
	void renderModel3D(Model3D* model3D, mathem::Transform* transform, render::Camera* camera, mathem::Transform* cameraTransform);
	
	inline void uploadSkybox(Skybox* skybox);
	inline void removeSkybox(Skybox* skybox);
	inline void renderSkybox(Skybox* skybox, render::Camera* camera, mathem::Transform* cameraTransform);
}

//==================================================================
//       Include your Renderer API implementation here             |
//==================================================================
#include "Renderer/src/implementations/RendererOpenGL/RendererOpenGL.h"
//==================================================================

#endif