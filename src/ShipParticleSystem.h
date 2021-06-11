#pragma once

#include <glad/glad.h>

#include "Actor.h"
#include "AnimatedActor.h"
#include "Program.h"

constexpr float PARTICLE_OFFSET_SCALE = 0.37f;

class ShipParticleSystem : public Actor
{
public:
	ShipParticleSystem(const AnimatedActor& ship, EditorState& editorState, const std::string& name) :
		Actor(editorState, name),
		m_ship(ship)
	{
	}

	void init(const std::string& resourceDirectory) override;
	void update(float deltaTime) override;
	void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime) override;

private:
	const AnimatedActor& m_ship;

	std::shared_ptr<Program> m_shaderProgram;

	GLuint m_textureId = 0;
	GLuint m_vertexArrayId = 0;
	GLuint m_positionBufferId = 0;
	GLuint m_normalBufferId = 0;
	GLuint m_texCoordBufferId = 0;
	GLuint m_indexBufferId = 0;
	GLuint m_instanceBufferId = 0;

	int m_spawnedParticleCount = 0;
	float m_totalTime = 0.0f;
	float m_lastSpawnTime = -1.0f; // Indicates no spawns have happened.
	glm::mat4 m_lastShipMatrix = glm::mat4(1.0f);

	glm::vec3 m_particleOffsets[4] =
	{
		glm::vec3(0.0f, 0.0f, 8.3f) * PARTICLE_OFFSET_SCALE,
		glm::vec3(0.0f, 1.9f, 5.5f) * PARTICLE_OFFSET_SCALE,
		glm::vec3(-2.175f, -0.6f, 5.5f) * PARTICLE_OFFSET_SCALE,
		glm::vec3(2.175f, -0.6f, 5.5f) * PARTICLE_OFFSET_SCALE,
	};
};

