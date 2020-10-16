
//----------------------------------------------------------------------------------------
#define SCREEN_WIDTH		1280
#define SCREEN_HEIGHT		720
//----------------------------------------------------------------------------------------

//#define DEBUG_ON

#include "MiteVox.h"

void waveModel_Script(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data)
{
	MiteVox_Scene* scene = (MiteVox_Scene*)data;
	mathem::Transform* transform =
		(mathem::Transform*)_ecs->componentManagers[TRANSFORM_COMPONENT].getComponentLocation(entityID);

	//transform->angleX += 30 * scene->dt;
	//transform->angleY += 60 * scene->dt;
	transform->y = 90 * (float)sin(scene->currentTime * 4 + transform->x + transform->z);
}

void rotateModel_Script(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data)
{
	MiteVox_Scene* scene = (MiteVox_Scene*)data;
	mathem::Transform* transform =
		(mathem::Transform*)_ecs->componentManagers[TRANSFORM_COMPONENT].getComponentLocation(entityID);

	transform->y = 270 * (float)sin(scene->currentTime * 2) + 350;
}

void spawnPointLight(float x, float y, float z, 
					float r, float g, float b, 
					MiteVox_Scene* gameScene, fileio::Image* texture, 
					render::Mesh3D* mesh, NativeScript_ECS* script = nullptr)
{
	unsigned long tmpID = gameScene->main_ECS->addEntity();
	mathem::Transform tmpTransform = { 1, 1, 1, 0, 0, 0, x, y, z };
	gameScene->main_ECS->attachComponent<mathem::Transform>(tmpID, TRANSFORM_COMPONENT, &tmpTransform);

	render::Material* light = new render::Material();
	light->ambient = { r, g, b };
	light->diffuse = { 0, 0, 0 };
	light->specular = { 0, 0, 0 };
	light->specularExponent = 1;
	light->albedoMap = *texture;
	light->metallicMap = *texture;
	render::Model3D* cube = new render::Model3D(mesh, 5, 5, 5, 0, 0, 0, 0, 0, 0);
	cube->material = light;
	cube->shaderID = 0;

	gameScene->main_ECS->attachComponent<render::Model3D>(tmpID, MODEL3D_COMPONENT, cube);
	render::PointLight tmpPointLight = { { 0 }, { r, g, b, 1 },  1, 0.0007f, 0.00002f };
	gameScene->main_ECS->attachComponent<render::PointLight>(tmpID, POINTLIGHT_COMPONENT, &tmpPointLight);

	if (script)
	{
		gameScene->main_ECS->attachComponent<NativeScript_ECS>(tmpID, NATIVE_SCRIPT_COMPONENT, script);
	}
}



extern render::RendererSettings RendererSettingsDebug;

int main(int argc, char* argv[])
{
	// Create the main scene.
	MiteVox_Scene* gameScene = new MiteVox_Scene(nullptr, nullptr, nullptr);
	gameScene->init();
	
	// Create 3D-model objects.
	render::Mesh3D* mCube = nullptr;
#ifdef DEBUG_ON
	fileio::fileLoader.loadAndParseAsync(
		"..\\Renderer\\assets\\cube.obj", (void**)&mCube, render::parseMesh_OBJ);
#else
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\engine\\Renderer\\assets\\cube.obj", (void**)&mCube, render::parseMesh_OBJ);
#endif
	fileio::fileLoader.awaitAll();

	// Load images (textures).
	fileio::Image* white;
	fileio::Image* white_rough;
	fileio::Image* light_grey;
	fileio::Image* black;
	fileio::Image* UVchecker0;
	fileio::Image* UVchecker1;
	fileio::Image* SPECchecker0;
#ifdef DEBUG_ON
	fileio::fileLoader.loadAndParseAsync(
		"..\\Renderer\\assets\\textures\\UVchecker0.png", (void**)&UVchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\Renderer\\assets\\textures\\UVchecker1.png", (void**)&UVchecker1, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\Renderer\\assets\\textures\\SPECchecker0.png", (void**)&SPECchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\Renderer\\assets\\textures\\white.png", (void**)&white, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\Renderer\\assets\\textures\\white_rough.png", (void**)&white_rough, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\Renderer\\assets\\textures\\light_grey.png", (void**)&light_grey, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\Renderer\\assets\\textures\\black.png", (void**)&black, fileio::loadImage);
#else
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\engine\\Renderer\\assets\\textures\\UVchecker0.png", (void**)&UVchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\engine\\Renderer\\assets\\textures\\UVchecker1.png", (void**)&UVchecker1, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\engine\\Renderer\\assets\\textures\\SPECchecker0.png", (void**)&SPECchecker0, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\engine\\Renderer\\assets\\textures\\white.png", (void**)&white, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\engine\\Renderer\\assets\\textures\\white_rough.png", (void**)&white_rough, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\engine\\Renderer\\assets\\textures\\light_grey.png", (void**)&light_grey, fileio::loadImage);
	fileio::fileLoader.loadAndParseAsync(
		"..\\..\\..\\engine\\Renderer\\assets\\textures\\black.png", (void**)&black, fileio::loadImage);
#endif
	fileio::fileLoader.awaitAll();

	render::Material* darkSomething = new render::Material();
	darkSomething->ambient = { 0.0f, 0.0f, 0.0f };
	darkSomething->diffuse = { 0.7f, 0.7f, 0.7f };
	darkSomething->specular = { 0.5f, 0.5f, 0.5f };
	darkSomething->specularExponent = 5;
	darkSomething->albedoMap = *SPECchecker0;
	darkSomething->metallicMap = *white;

	render::Material* silver = new render::Material();
	silver->ambient = { 0.19225f, 0.19225f, 0.19225f };
	silver->diffuse = { 0.50754f, 0.50754f, 0.50754f };
	silver->specular = { 0.508273f, 0.508273f, 0.508273f };
	silver->specularExponent = 0.4f * 128;
	silver->albedoMap = *white;
	silver->metallicMap = *white;


	NativeScript_ECS waveScript = { doNothing, waveModel_Script, doNothing };
	NativeScript_ECS rotateScript = { doNothing, rotateModel_Script, doNothing };

	//----------------------------------------------------------------------------------------

	int maxX = 2;
	int maxY = 2;
	for (int i = -maxX; i <= maxX; i++)
	{
		for (int j = -maxY; j <= maxY; j++)
		{
			unsigned long tmpID = gameScene->main_ECS->addEntity();
			mathem::Transform tmpTransform = { 1, 1, 1, 0, 0, 0, (float)50 * i, 0, (float)50 * j };
			gameScene->main_ECS->attachComponent<mathem::Transform>(tmpID, TRANSFORM_COMPONENT, &tmpTransform);

			render::Model3D* cube = new render::Model3D(mCube, 25, 25, 25, 0, 90, 0, 0, 0, 0);
			cube->material = darkSomething;
			gameScene->main_ECS->attachComponent<NativeScript_ECS>(tmpID, NATIVE_SCRIPT_COMPONENT, &waveScript);
			cube->shaderID = 0;
			
			gameScene->main_ECS->attachComponent<render::Model3D>(tmpID, MODEL3D_COMPONENT, cube);
		}
	}

	spawnPointLight(0, 300, 0, 1, 0.7f, 0, gameScene, white, mCube, &rotateScript);

	spawnPointLight(150, 150, 150, 1, 0, 0, gameScene, white, mCube);
	spawnPointLight(-150, 150, 150, 0, 1, 0, gameScene, white, mCube);
	spawnPointLight(-150, 150, -150, 0, 0, 1, gameScene, white, mCube);
	spawnPointLight(150, 150, -150, 0.6f, 0.2f, 0.7f, gameScene, white, mCube);

	spawnPointLight(150, -150, 150, 1, 0, 0, gameScene, white, mCube);
	spawnPointLight(-150, -150, 150, 0, 1, 0, gameScene, white, mCube);
	spawnPointLight(-150, -150, -150, 0, 0, 1, gameScene, white, mCube);
	spawnPointLight(150, -150, -150, 0.6f, 0.2f, 0.7f, gameScene, white, mCube);
	
	while (!glfwWindowShouldClose(render::RendererSettingsDebug.getWindow()))
	{
		GLFWwindow* window = render::RendererSettingsDebug.getWindow();
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		float cubeSize = 176;
		render::ColorRGBAf grey = { 0.7, 0.7, 0.7, 1 };
		render::ColorRGBAf red = { 1, 0, 0, 1 };
		render::ColorRGBAf green = { 0, 1, 0, 1 };
		render::ColorRGBAf blue = { 0, 0, 1, 1 };
		render::ColorRGBAf purple = { 0.6f, 0.2f, 0.7f, 1 };
		render::Point point1 = { {cubeSize, cubeSize, cubeSize}, red };
		render::Point point2 = { {cubeSize, -cubeSize, cubeSize}, red };
		render::Point point3 = { {cubeSize, -cubeSize, -cubeSize}, purple };
		render::Point point4 = { {cubeSize, cubeSize, -cubeSize}, purple };
		render::Point point5 = { {-cubeSize, cubeSize, -cubeSize}, blue };
		render::Point point6 = { {-cubeSize, -cubeSize, -cubeSize}, blue };
		render::Point point7 = { {-cubeSize, -cubeSize, cubeSize}, green };
		render::Point point8 = { {-cubeSize, cubeSize, cubeSize}, green };

		render::drawLine(point1, point2);
		render::drawLine(point2, point3);
		render::drawLine(point3, point4);
		render::drawLine(point4, point5);
		render::drawLine(point5, point6);
		render::drawLine(point6, point7);
		render::drawLine(point7, point8);
		render::drawLine(point8, point1);

		render::drawLine(point1, point4);
		render::drawLine(point3, point6);
		render::drawLine(point5, point8);
		render::drawLine(point2, point7);

		point1.color = red;
		point1.pos = { -cubeSize * 3, -cubeSize, cubeSize };
		point6.color = blue;
		point7.color = green;
		render::drawTriangle(point1, point6, point7);

		render::Point point = { {0, 0, 0}, {0.15, 0.15, 0.15, 1} };
		render::drawCross(point, 10000);
		
		gameScene->update();
	}
	glfwTerminate();

	//getchar();

	return 0;
}