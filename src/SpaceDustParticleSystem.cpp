#include "SpaceDustParticleSystem.h"

#include <iostream>
#include <glm/gtc/random.hpp>

#include "GLSL.h"
#include "SmartTexture.h"

constexpr int INSTANCE_COUNT = 100000;
constexpr float PARTICLE_SPAN = 200;

void SpaceDustParticleSystem::init(const std::string& resourceDirectory)
{
	m_shaderProgram = std::make_shared<Program>();
	m_shaderProgram->setVerbose(true);
	m_shaderProgram->setShaderNames(resourceDirectory + "space_dust_vertex.glsl", resourceDirectory + "space_dust_frag.glsl");

	if (!m_shaderProgram->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}

	m_shaderProgram->addUniform("P");
	m_shaderProgram->addUniform("V");
	m_shaderProgram->addUniform("M");
	m_shaderProgram->addUniform("campos");
	m_shaderProgram->addAttribute("vertPos");
	m_shaderProgram->addAttribute("vertNor");
	m_shaderProgram->addAttribute("vertTex");
	m_shaderProgram->addUniform("time");
	m_shaderProgram->addAttribute("instancePos");

	glGenVertexArrays(1, &m_vertexArrayId);
	glBindVertexArray(m_vertexArrayId);

	// Vertices
	glGenBuffers(1, &m_positionBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferId);

	GLfloat vertices[] =
	{
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Normals
	glGenBuffers(1, &m_normalBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferId);

	GLfloat normals[] =
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tex coords (TODO: Do we even need this?)
	glGenBuffers(1, &m_texCoordBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBufferId);

	glm::vec2 texCoords[] =
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Indices
	glGenBuffers(1, &m_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

	GLushort indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Instance positions
	glGenBuffers(1, &m_instanceBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_instanceBufferId);

	glm::vec4* instancePositions = new glm::vec4[INSTANCE_COUNT];

	for (int i = 0; i < INSTANCE_COUNT; i++)
	{
		instancePositions[i] = glm::vec4(glm::linearRand(glm::vec3(0.0f), glm::vec3(PARTICLE_SPAN)), 0.0f);
	}

	glBufferData(GL_ARRAY_BUFFER, INSTANCE_COUNT * sizeof(glm::vec4), instancePositions, GL_STATIC_DRAW);

	int positionShaderLocation = glGetAttribLocation(m_shaderProgram->pid, "instancePos");

	for (int i = 0; i < INSTANCE_COUNT; i++)
	{
		glVertexAttribPointer(
			positionShaderLocation + i,
			4, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec4),
			(void*)(sizeof(glm::vec4) * i));

		glEnableVertexAttribArray(positionShaderLocation + i);
		glVertexAttribDivisor(positionShaderLocation + i, 1);
	}

	glGetError();
	glBindVertexArray(0);
}

void SpaceDustParticleSystem::update(float deltaTime)
{
}

void SpaceDustParticleSystem::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime)
{
	m_shaderProgram->bind();

	glBindVertexArray(m_vertexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

	glm::mat4 viMatrix = glm::transpose(vMatrix);
	viMatrix[0][3] = 0;
	viMatrix[1][3] = 0;
	viMatrix[2][3] = 0;

	glm::mat4 mMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.025f)) * viMatrix;
	glUniformMatrix4fv(m_shaderProgram->getUniform("P"), 1, GL_FALSE, &pMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("V"), 1, GL_FALSE, &vMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("M"), 1, GL_FALSE, &mMatrix[0][0]);
	glUniform1f(m_shaderProgram->getUniform("time"), getEditorState().totalTime);
	glUniform3fv(m_shaderProgram->getUniform("campos"), 1, &getEditorState().cameraPosition[0]);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, INSTANCE_COUNT);

	glBindVertexArray(0);

	m_shaderProgram->unbind();
}
