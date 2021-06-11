#include "ShipParticleSystem.h"

#include <iostream>

#include "GLSL.h"
#include "SmartTexture.h"

constexpr int INSTANCE_COUNT = 100000;
constexpr int X_TILE_COUNT = 4;
constexpr int Y_TILE_COUNT = 4;
constexpr float SPAWN_RATE = 0.001f;

void ShipParticleSystem::init(const std::string& resourceDirectory)
{
	std::string str = resourceDirectory + "plasmaball.png";
	int width, height, channels;
	unsigned char* data = gettextureData(const_cast<char*>(str.c_str()), width, height, channels);
	glGenTextures(1, &m_textureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	m_shaderProgram = std::make_shared<Program>();
	m_shaderProgram->setVerbose(true);
	m_shaderProgram->setShaderNames(resourceDirectory + "plasma_vertex.glsl", resourceDirectory + "plasma_fragment.glsl");

	if (!m_shaderProgram->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}

	m_shaderProgram->addUniform("P");
	m_shaderProgram->addUniform("V");
	m_shaderProgram->addUniform("M");
	m_shaderProgram->addUniform("campos");
	m_shaderProgram->addUniform("tileOffset");
	m_shaderProgram->addUniform("nextTileOffset");
	m_shaderProgram->addUniform("lerpAmount");
	m_shaderProgram->addUniform("totalTime");
	m_shaderProgram->addAttribute("vertPos");
	m_shaderProgram->addAttribute("vertNor");
	m_shaderProgram->addAttribute("vertTex");
	m_shaderProgram->addAttribute("instancePos");

	GLuint tex1Location = glGetUniformLocation(m_shaderProgram->pid, "tex");
	GLuint tex2Location = glGetUniformLocation(m_shaderProgram->pid, "tex2");

	glUseProgram(m_shaderProgram->pid);
	glUniform1i(tex1Location, 0);
	glUniform1i(tex2Location, 1);

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
		// Initial particles are far out of view.
		instancePositions[i] = glm::vec4(-1000.0f, -1000.0f, -1000.0f, 0.0f);
	}

	glBufferData(GL_ARRAY_BUFFER, INSTANCE_COUNT * sizeof(glm::vec4), instancePositions, GL_DYNAMIC_DRAW);

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

void ShipParticleSystem::update(float deltaTime)
{
	m_totalTime += deltaTime;

	if (m_lastSpawnTime < 0)
	{
		m_lastShipMatrix = m_ship.computeMatrix();
		m_lastSpawnTime = m_totalTime;
	}

	float timeSinceLastSpawn = m_totalTime - m_lastSpawnTime;

	if (timeSinceLastSpawn < SPAWN_RATE)
	{
		return;
	}

	glm::mat4 shipMatrix = m_ship.computeMatrix();

	if (shipMatrix == m_lastShipMatrix)
	{
		m_lastSpawnTime = m_totalTime;
		return;
	}

	float newSpawnTime = m_lastSpawnTime;

	while (newSpawnTime <= m_totalTime - SPAWN_RATE)
	{
		newSpawnTime += SPAWN_RATE;

		for (int i = 0; i < sizeof(m_particleOffsets) / sizeof(glm::vec3); i++)
		{
			glm::mat4 lastSpawnMatrix = m_lastShipMatrix * glm::translate(glm::mat4(1.0f), m_particleOffsets[i]);
			glm::mat4 spawnMatrix = shipMatrix * glm::translate(glm::mat4(1.0f), m_particleOffsets[i]);

			glm::vec3 lastSpawnPosition = lastSpawnMatrix[3];
			glm::vec3 spawnPosition = spawnMatrix[3];

			glm::vec3 interpolatedPosition = glm::mix(lastSpawnPosition, spawnPosition, (newSpawnTime - m_lastSpawnTime) / (m_totalTime - m_lastSpawnTime));
			glm::vec4 newParticle = glm::vec4(interpolatedPosition, newSpawnTime);

			// HACK: This is kinda terrible but it seems to work well.
			// If it causes problems (light lagging behind the ship), then change this.
			getEditorState().lightPositions[i + 1] = interpolatedPosition;

			glBindBuffer(GL_ARRAY_BUFFER, m_instanceBufferId);

			// We update the array back-to-front so the render order of the particles is correct when looking from the back of the ship.
			glBufferSubData(GL_ARRAY_BUFFER, (INSTANCE_COUNT - 1 - (m_spawnedParticleCount % INSTANCE_COUNT)) * sizeof(glm::vec4), sizeof(glm::vec4), &newParticle);

			m_spawnedParticleCount++;
		}
	}

	m_lastSpawnTime = newSpawnTime;
	m_lastShipMatrix = shipMatrix;
}

glm::ivec2 getTileOffset(int frameNumber)
{
	return glm::ivec2(frameNumber % X_TILE_COUNT, (frameNumber / X_TILE_COUNT) % Y_TILE_COUNT);
}

void ShipParticleSystem::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime)
{
	m_shaderProgram->bind();

	auto scaledTotalTime = m_totalTime * 20.0f;

	auto frameNumber = (int)scaledTotalTime;
	auto lerpAmount = scaledTotalTime - frameNumber;

	auto tileOffset = getTileOffset(frameNumber);
	auto nextTileOffset = getTileOffset(frameNumber + 1);

	glBindVertexArray(m_vertexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

	glm::mat4 viMatrix = glm::transpose(vMatrix);
	viMatrix[0][3] = 0;
	viMatrix[1][3] = 0;
	viMatrix[2][3] = 0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glm::mat4 mMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f)) * viMatrix;
	glUniformMatrix4fv(m_shaderProgram->getUniform("P"), 1, GL_FALSE, &pMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("V"), 1, GL_FALSE, &vMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("M"), 1, GL_FALSE, &mMatrix[0][0]);
	glUniform1f(m_shaderProgram->getUniform("totalTime"), m_totalTime);
	glUniform2iv(m_shaderProgram->getUniform("tileOffset"), 1, &tileOffset[0]);
	glUniform2iv(m_shaderProgram->getUniform("nextTileOffset"), 1, &nextTileOffset[0]);
	glUniform1f(m_shaderProgram->getUniform("lerpAmount"), lerpAmount);
	glUniform3fv(m_shaderProgram->getUniform("campos"), 1, &getEditorState().cameraPosition[0]);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, INSTANCE_COUNT);

	glBindVertexArray(0);

	m_shaderProgram->unbind();
}
