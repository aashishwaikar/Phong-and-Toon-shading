// STL
#include <iostream>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <DiffuseMaterial.hpp>
#include <PhongMaterial.hpp>
#include <ToonMaterial.hpp>
#include "RenderObject.hpp"
#include "ModelLoader.hpp"

GLFWwindow* g_window;
float g_window_width = 1024.0f;
float g_window_height = 768.0f;
int g_framebuffer_width = 1440;
int g_framebuffer_height = 1080;

Engine::Camera *mainCamera;

static bool m = false;
static bool day = true;
static bool toon = false;

constexpr float PI = 3.1415926f;
constexpr float CAMERA_ROTATE_SPEED = 0.5f;
static float camera_direction = -0.5 * PI;

static PhongMaterial *phongMaterial;
static ToonMaterial *toonMaterial;

static std::vector<Engine::RenderObject *> objects;

glm::vec3 genColor(unsigned int color)
{
	return glm::vec3((float)((color & 0xFF0000) >> 16) / 255.0f, (float)((color & 0xFF00) >> 8) / 255.0f, (float)(color & 0xFF) / 255.0f);
}

static auto gray    = genColor(0x7a7a7a);
static auto sky_color = genColor(0xEDFAFD);

static std::vector<Light> lights;

static void KeyboardCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{	
	if (a_action == GLFW_PRESS)
	{
		switch (a_key)
		{
		case GLFW_KEY_N:
			day = !day;
			if (day)
			{
				lights[0].enabled = true;
				lights[2].enabled = false;

				sky_color = genColor(0xEDFAFD);
				glClearColor(sky_color.x, sky_color.y, sky_color.z, 1);
			}
			else
			{
				lights[0].enabled = false;
				lights[2].enabled = true;

				sky_color = genColor(0x070B34);
				glClearColor(sky_color.x, sky_color.y, sky_color.z, 1);
			}
			break;
		case GLFW_KEY_T:
			toon = !toon;
			if (toon)
			{
				for (auto obj : objects)
					obj->SetMaterial(toonMaterial);
			}
			else
			{
				for (auto obj : objects)
					obj->SetMaterial(phongMaterial);
			}
			break;
		}
	}
}

int main(int argc, char** argv)
{
	// Initialize GLFW library
	if (!glfwInit())
	{
		return -1;
	}

	// Create window and OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, false);

	/* Create a windowed mode window and its OpenGL context */
	g_window = glfwCreateWindow(g_window_width, g_window_height, "HW3", NULL, NULL);
	if (!g_window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(g_window);

	// Initialize GLEW library
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(glew_error) << std::endl;
		exit(1);
	}

	mainCamera = new Engine::Camera(
		glm::vec3(0.0f, 0.0f, 2.0f),
		glm::rotate(
			glm::rotate(glm::mat4(1.0f), 0.5f * PI, glm::vec3(1, 0, 0)),
			camera_direction, glm::vec3(0, 1, 0)
		)
	);
	mainCamera->SetProjection(g_window_width / g_window_height, 70.0, 0.1, 120);

	Engine::ModelLoader *loader = new Engine::ModelLoader("Resources\\Models\\Couple.obj");
	Engine::Mesh *couple_mesh = new Engine::Mesh();
	loader->GenerateMesh(couple_mesh, 0);

	phongMaterial = new PhongMaterial();
	toonMaterial = new ToonMaterial();
	phongMaterial->CreateMaterial();
	toonMaterial->CreateMaterial(false);

	Engine::RenderObject *CoupleObj = new Engine::RenderObject(couple_mesh, phongMaterial);
	CoupleObj->GetTransform()->SetPosition(glm::vec3(30, 0, -5));
	CoupleObj->GetTransform()->SetScale(glm::vec3(2, 2, 2));

	objects.push_back(CoupleObj);
	
	lights.push_back(Light());
	lights[0].type = DirectionalLight;
	lights[0].enabled = true;
	lights[0].dir = glm::vec3(-1, -1, -1);
	lights[0].diffused = sky_color;
	lights[0].specular = sky_color;
	
	lights.push_back(Light());
	lights[1].type = PointLight;
	lights[1].enabled = false;
	lights[1].transform.SetPosition(glm::vec3(10, 0, 3));
	lights[1].diffused = glm::vec3(200, 200, 200);
	lights[1].specular = glm::vec3(200, 200, 200);
	
	lights.push_back(Light());
	lights[2].type = Spotlight;
	lights[2].enabled = false;
	lights[2].transform.SetPosition(glm::vec3(0, 0, 0));
	lights[2].diffused = glm::vec3(3, 3, 3);
	lights[2].specular = glm::vec3(3, 3, 3);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(g_window, KeyboardCallback);

	float prev_time = (float)glfwGetTime();
	float avg_elapsed_time = 1.0f / 60.0f;

	glClearColor(sky_color.x, sky_color.y, sky_color.z, 1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		float total_time = (float)glfwGetTime();
		float elapsed_time = total_time - prev_time;
		avg_elapsed_time = avg_elapsed_time * (59.0 / 60.0) + elapsed_time * (1.0 / 60.0);
		prev_time = total_time;

		glClear(GL_COLOR_BUFFER_BIT | (m ? 0 : GL_DEPTH_BUFFER_BIT));

		//spotlight_animation
		lights[2].dir = glm::vec3(
			5 * cos(0.4 * total_time),
			5 * sin(0.4 * total_time),
			2
		);

		mainCamera->GetTransform()->SetOrientation(
			glm::rotate(
				glm::rotate(glm::mat4(1.0f), 0.5f * PI, glm::vec3(1, 0, 0)), 
				camera_direction, glm::vec3(0, 1, 0)
			)
		);

		if (!toon)
		{
			phongMaterial->UpdateLight(lights);
			phongMaterial->UpdateSpecularReflectance(glm::vec3(0, 0, 0));
			phongMaterial->UpdateAmbientReflectance(gray);
			phongMaterial->UpdateDiffuseReflectance(gray);
			CoupleObj->Render(mainCamera);
		}
		else
		{
			toonMaterial->UpdateLight(lights);
			toonMaterial->UpdateDiffuseReflectance(gray);
			CoupleObj->Render(mainCamera);
		}


		/* Swap front and back buffers */
		glfwSwapBuffers(g_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}