/*
CPE/CSC 474 Lab base code Eckhardt/Dahl
based on CPE/CSC 471 Lab base code Wood/Dunn/Eckhardt
*/

#include <iostream>
#include <fstream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "WindowManager.h"
#include "Shape.h"
#include "line.h"
#include "skmesh.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// assimp
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/vector3.h"
#include "assimp/scene.h"
#include <assimp/mesh.h>

#include "stb_image.h"

#include "RenderPipeline.h"
#include "EditorState.h"
#include "Actor.h"
#include "CameraActor.h"
#include "Ship.h"
#include "Asteroid.h"
#include "ShipParticleSystem.h"
#include "SpaceDustParticleSystem.h"
#include "Sun.h"

#define MESHSIZE 1000

using namespace std;
using namespace glm;
using namespace Assimp;

shared_ptr<Shape> shape;
shared_ptr<Shape> plane;

ofstream file;
int renderstate = 1;

double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime =glfwGetTime();
	double difference = actualtime- lasttime;
	lasttime = actualtime;
	return difference;
}

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog, heightshader, skyprog, linesshader, pplane, skinningprog, pbrProgram;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our box to OpenGL
	//GLuint VertexBufferID, VertexNormDBox, VertexTexBox, IndexBufferIDBox;
	GLuint MeshPosID, MeshTexID, IndexBufferIDBox;

	std::shared_ptr<RenderPipeline> renderPipeline;

	//texture data
	GLuint Texture;
	GLuint Texture2, HeightTex;
	
	SkinnedMesh skmesh;

	EditorState m_editorState =
	{
		1979.0f / 30.0f,//0.0f,							// Total time
		0.0f,							// Playback speed
		-1,								// Selected actor index
		glm::vec3(0.0f, 0.0f, 0.0f),	// Camera position
		false,							// Debugging enabled
		{								// Light positions
			glm::vec3(100.0f, 1000.0f, 0.0f),		// Sun
			glm::vec3(-100000000.0f),				// Plasma light (initially far away, simulating the engines being off)
			glm::vec3(-100000000.0f),				// Plasma light
			glm::vec3(-100000000.0f),				// Plasma light
			glm::vec3(-100000000.0f),				// Plasma light
		},
		{								// Light colors
			glm::vec3(1.0f, 1.0f, 0.7f) * 500000.0f,// Sun
			glm::vec3(0.0f, 0.25f, 1.0f) * 4.0f,	// Plasma light
			glm::vec3(0.0f, 0.25f, 1.0f) * 0.25f,	// Plasma light
			glm::vec3(0.0f, 0.25f, 1.0f) * 0.25f,	// Plasma light
			glm::vec3(0.0f, 0.25f, 1.0f) * 0.25f,	// Plasma light
		}
	};

	std::shared_ptr<CameraActor> m_cameraActor;

	std::vector<std::shared_ptr<Actor>> m_actors;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
		{
			m_editorState.playbackSpeed = 0.0f;
			m_editorState.totalTime = 0.0f;
			return;
		}

		if (key == GLFW_KEY_H && action == GLFW_PRESS)
		{
			m_editorState.debuggingEnabled = !m_editorState.debuggingEnabled;

			if (m_editorState.debuggingEnabled)
			{
				std::cout << "Debug mode enabled.\n";
			}
			else
			{
				std::cout << "Debug mode disabled.\n";
			}
		}

		if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_ALT))
		{
			// CTRL/ALT+KEY events are intercepted by actors.
			return;
		}

		if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		{
			int roundPrecision = (mods & GLFW_MOD_SHIFT)
				? 100
				: 10;

			m_editorState.totalTime = floor(m_editorState.totalTime * roundPrecision + 1.5f) / roundPrecision;
			m_editorState.playbackSpeed = 0.0f;

			std::cout << "Snapped time to " << m_editorState.totalTime << "s.\n";
		}

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			m_editorState.playbackSpeed = m_editorState.playbackSpeed == 0.0f
				? 1.0f
				: 0.0f;
		}
		else
		{
			auto playbackSpeedScale = mods & GLFW_MOD_SHIFT
				? 0.2f
				: 10.0f;

			if (key == GLFW_KEY_LEFT)
			{
				if (action == GLFW_PRESS)
				{
					m_editorState.playbackSpeed = -playbackSpeedScale;
				}
				else if (m_editorState.playbackSpeed < 0.0f && action == GLFW_RELEASE)
				{
					m_editorState.playbackSpeed = 0.0f;
				}
			}
			else if (key == GLFW_KEY_RIGHT)
			{
				if (action == GLFW_PRESS)
				{
					m_editorState.playbackSpeed = playbackSpeedScale;
				}
				else if (m_editorState.playbackSpeed > 0.0f && action == GLFW_RELEASE)
				{
					m_editorState.playbackSpeed = 0.0f;
				}
			}
		}
	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}
	/*Note that any gl calls must always happen after a GL state is initialized */

	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom(const std::string& resourceDirectory)
	{
		if (!skmesh.LoadMesh(resourceDirectory + "/boblampclean.md5mesh")) {
			printf("Mesh load failed\n");
			return;
		}

		Assimp::Importer importer;
		const aiScene* scene;
		
		std::string path = "";
		scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		
		// Initialize mesh.
		shape = make_shared<Shape>();
		shape->loadMesh(resourceDirectory + "/sphere.obj");
		shape->resize();
		shape->init();

		plane = make_shared<Shape>();
		plane->loadMesh(resourceDirectory + "/t800.obj");
		plane->resize();
		plane->init();

		int width, height, channels;
		char filepath[1000];

		//texture 1
		string str = resourceDirectory + "/explosion.jpg";
		strcpy(filepath, str.c_str());
		unsigned char* data = gettextureData(filepath, width, height, channels);
		glGenTextures(1, &Texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		//texture 2
		str = resourceDirectory + "/skybox.png";
		strcpy(filepath, str.c_str());
		data = gettextureData(filepath, width, height, channels);
		glGenTextures(1, &Texture2);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//[TWOTEXTURES]
		//set the 2 textures to the correct samplers in the fragment shader:
		GLuint Tex1Location = glGetUniformLocation(prog->pid, "tex");//tex, tex2... sampler in the fragment shader
		GLuint Tex2Location = glGetUniformLocation(prog->pid, "tex2");
		// Then bind the uniform samplers to texture units:
		glUseProgram(prog->pid);
		glUniform1i(Tex1Location, 0);
		glUniform1i(Tex2Location, 1);

		Tex1Location = glGetUniformLocation(skyprog->pid, "tex");//tex, tex2... sampler in the fragment shader
		Tex2Location = glGetUniformLocation(skyprog->pid, "tex2");
		// Then bind the uniform samplers to texture units:
		glUseProgram(skyprog->pid);
		glUniform1i(Tex1Location, 0);
		glUniform1i(Tex2Location, 1);

		//glUseProgram(pbrProgram->pid);
		//GLuint skyLocation = glGetUniformLocation(pbrProgram->pid, "skybox");
		//glUniform1i(skyLocation, 0);
		//glUseProgram(0);

		renderPipeline = std::make_shared<RenderPipeline>(resourceDirectory + "/");

		auto asteroidsShape = std::make_shared<Shape>();

		auto asteroidsMtlDir = resourceDirectory + "/Asteroids/";
		asteroidsShape->loadMesh(resourceDirectory + "/Asteroids/Asteroids.obj", &asteroidsMtlDir, stbi_load);
		asteroidsShape->init();

		auto planetShape = std::make_shared<Shape>();
		auto planetMtlDir = resourceDirectory + "/Scene3/Saturn/";
		planetShape->loadMesh(resourceDirectory + "/Scene3/Saturn/Saturn.obj", &asteroidsMtlDir, stbi_load);
		planetShape->init();

		//auto asteroidsProgram = std::make_shared<Program>();
		//asteroidsProgram->setVerbose(true);
		//asteroidsProgram->setShaderNames(resourceDirectory + "/asteroid_vertex.glsl", resourceDirectory + "/asteroid_frag.glsl");
		//if (!asteroidsProgram->init())
		//{
		//	std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		//	exit(1);
		//}
		//asteroidsProgram->addUniform("P");
		//asteroidsProgram->addUniform("V");
		//asteroidsProgram->addUniform("M");
		//asteroidsProgram->addUniform("campos");
		//asteroidsProgram->addAttribute("vertPos");
		//asteroidsProgram->addAttribute("vertNor");
		//asteroidsProgram->addAttribute("vertTex");

		auto& asteroidsProgram = pbrProgram;

		auto ship = std::make_shared<Ship>(pbrProgram, Texture2, m_editorState, "Test");
		m_cameraActor = std::make_shared<CameraActor>(*ship, m_editorState, "Camera_unanimated");
		m_actors.push_back(ship);
		m_actors.push_back(std::make_shared<Sun>(planetShape, m_editorState, "Sun"));
		m_actors.push_back(m_cameraActor);
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(0.0f, 0.0f, 0.0f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids0"));
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(0.0f, 70.0f, 0.0f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids1"));
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(13.5084f, 138.684f, -0.000079f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids2"));
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(27.0167f, 207.368f, -0.000158f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids3"));
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(40.5251f, 276.053f, -0.000236f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids4"));
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(54.0334f, 344.737f, -0.000315f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids5"));
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(54.0334f, 414.737f, -0.000315f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids6"));
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(54.0334f, 484.737f, -0.000315f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids7"));
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(54.0334f, 554.737f, -0.000315f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids8"));
		//m_actors.push_back(std::make_shared<Asteroid>(glm::vec3(54.0334f, 624.737f, -0.000315f), asteroidsShape, asteroidsProgram, m_editorState, "Asteroids9"));
		m_actors.push_back(std::make_shared<ShipParticleSystem>(*ship, m_editorState, "ShipParticleSystem"));
		m_actors.push_back(std::make_shared<SpaceDustParticleSystem>(m_editorState, "SpaceDustParticleSystem"));

		for (auto& actor : m_actors)
		{
			actor->init(resourceDirectory + "/");
		}
	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_DEPTH_TEST);
		// Initialize the GLSL program.
		prog = std::make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
		if (!prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("campos");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");

		skyprog = std::make_shared<Program>();
		skyprog->setVerbose(true);
		skyprog->setShaderNames(resourceDirectory + "/sky_vertex.glsl", resourceDirectory + "/sky_fragment.glsl");
		if (!skyprog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		skyprog->addUniform("P");
		skyprog->addUniform("V");
		skyprog->addUniform("M");
		skyprog->addAttribute("vertPos");
		skyprog->addAttribute("vertTex");

		pplane = std::make_shared<Program>();
		pplane->setVerbose(true);
		pplane->setShaderNames(resourceDirectory + "/plane_vertex.glsl", resourceDirectory + "/plane_frag.glsl");
		if (!pplane->init())
			{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
			}
		pplane->addUniform("P");
		pplane->addUniform("V");
		pplane->addUniform("M");
		pplane->addUniform("campos");
		pplane->addAttribute("vertPos");
		pplane->addAttribute("vertNor");
		pplane->addAttribute("vertTex");

		// Initialize the GLSL program.
		//heightshader = std::make_shared<Program>();
		//heightshader->setVerbose(true);
		//heightshader->setShaderNames(resourceDirectory + "/height_vertex.glsl", resourceDirectory + "/height_frag.glsl", resourceDirectory + "/geometry.glsl");
		//if (!heightshader->init())
		//{
		//	std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		//	exit(1);
		//}
		//heightshader->addUniform("P");
		//heightshader->addUniform("V");
		//heightshader->addUniform("M");
		//heightshader->addUniform("camoff");
		//heightshader->addUniform("campos");
		//heightshader->addUniform("morph");
		//heightshader->addAttribute("vertPos");
		//heightshader->addAttribute("vertTex");
		//heightshader->addUniform("bgcolor");
		//heightshader->addUniform("renderstate");

		skinningprog = std::make_shared<Program>();
		skinningprog->setVerbose(true);
		skinningprog->setShaderNames(resourceDirectory + "/skinning_vert.glsl", resourceDirectory + "/skinning_frag.glsl");
		if (!skinningprog->init())
			{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
			}
		skinningprog->addUniform("P");
		skinningprog->addUniform("V");
		skinningprog->addUniform("M");
		skinningprog->addUniform("campos");
		skinningprog->addAttribute("vertPos");
		skinningprog->addAttribute("vertNor");
		skinningprog->addAttribute("vertTex");
		skinningprog->addAttribute("BoneIDs");
		skinningprog->addAttribute("Weights");

		pbrProgram = std::make_shared<Program>();
		pbrProgram->setVerbose(true);
		pbrProgram->setShaderNames(resourceDirectory + "/pbr_vert.glsl", resourceDirectory + "/pbr_frag.glsl");
		if (!pbrProgram->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		pbrProgram->addUniform("P");
		pbrProgram->addUniform("V");
		pbrProgram->addUniform("M");
		//m_shaderProgram->addUniform("LS");
		//m_shaderProgram->addUniform("albedoMap");
		//m_shaderProgram->addUniform("roughnessMap");
		//m_shaderProgram->addUniform("metallicMap");
		//m_shaderProgram->addUniform("aoMap");
		pbrProgram->addUniform("albedo");
		pbrProgram->addUniform("roughness");
		pbrProgram->addUniform("metallic");
		pbrProgram->addUniform("ao");
		pbrProgram->addUniform("lightPositions");
		pbrProgram->addUniform("lightColors");
		pbrProgram->addUniform("viewPos");
		//pbrProgram->addUniform("skybox");
		pbrProgram->addAttribute("vertPos");
		pbrProgram->addAttribute("vertNor");
		pbrProgram->addAttribute("vertTex");
	}


	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
	void render()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		double frameTime = get_last_elapsed_time();
		//frameTime *= 0.5f;

		m_editorState.totalTime += frameTime * m_editorState.playbackSpeed;

		if (m_editorState.totalTime < 0.0f)
		{
			m_editorState.totalTime = 0.0f;
		}

		if (m_editorState.playbackSpeed != 0.0f)
		{
			std::cout << "Time: " << m_editorState.totalTime << "s\t\t\r";
		}

		static double totaltime = 0;
		totaltime += frameTime;

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width/(float)height;

		// TODO: Try implementing PBR.

		renderPipeline->preRender(windowManager->getHandle());

		for (auto& actor : m_actors)
		{
			actor->update(frameTime);
		}
		
		glm::mat4 V, M, P; //View, Model and Perspective matrix
		V = m_cameraActor->computeMatrix();//mycam.process(frametime);
		M = glm::mat4(1);
		// Apply orthographic projection....
		P = glm::ortho(-1 * aspect, 1 * aspect, -1.0f, 1.0f, -2.0f, 100.0f);		
		if (width < height)
			{
			P = glm::ortho(-1.0f, 1.0f, -1.0f / aspect,  1.0f / aspect, -2.0f, 100.0f);
			}
		// ...but we overwrite it (optional) with a perspective projection.
		//P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones
		constexpr float halfPi = glm::pi<float>() * 0.5f;
		float fov = 39.6f / aspect;
		P = glm::perspective((float)(fov / 90. * halfPi), (float)(aspect), 1.f, 10000.0f); //so much type casting... GLM metods are quite funny ones

		// Skybox
		glm::mat4 skyRotated = glm::rotate(glm::mat4(1), halfPi, vec3(1, 0, 0));
		glm::mat4 skyTranslated = glm::translate(glm::mat4(1.0f), -m_editorState.cameraPosition);
		glm::mat4 skyScaled = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
		M = skyTranslated * skyRotated * skyScaled;

		//M = TransSky  * RotateXSky * SSky;

		// Draw the sky using GLSL.
		skyprog->bind();		
		glUniformMatrix4fv(skyprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(skyprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(skyprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		//glUniform3fv(skyprog->getUniform("campos"), 1, &m_editorState.cameraPosition[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		glDisable(GL_DEPTH_TEST);
		shape->draw(skyprog);			//render!!!!!!!
		glEnable(GL_DEPTH_TEST);	
		skyprog->unbind();

		for (auto& actor : m_actors)
		{
			actor->render(P, V, frameTime);
		}

		renderPipeline->postRender();
	}

};
//******************************************************************************************
int main(int argc, char **argv)
{
	std::string resourceDir = "../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom(resourceDir);

	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}

