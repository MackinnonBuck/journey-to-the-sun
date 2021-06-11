#pragma once

#include <glad/glad.h>

#include "Actor.h"
#include "AnimatedActor.h"
#include "Program.h"

class SpaceDustParticleSystem : public Actor
{
public:
	SpaceDustParticleSystem(EditorState& editorState, const std::string& name) :
		Actor(editorState, name)
	{
	}

	void init(const std::string& resourceDirectory) override;
	void update(float deltaTime) override;
	void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime) override;

private:
	std::shared_ptr<Program> m_shaderProgram;

	GLuint m_vertexArrayId = 0;
	GLuint m_positionBufferId = 0;
	GLuint m_normalBufferId = 0;
	GLuint m_texCoordBufferId = 0;
	GLuint m_indexBufferId = 0;
	GLuint m_instanceBufferId = 0;
};

