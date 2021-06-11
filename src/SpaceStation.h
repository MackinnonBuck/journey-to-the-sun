#pragma once

#include <memory>

#include "Actor.h"
#include "Shape.h"
#include "Program.h"
#include "SmartTexture.h"

class SpaceStation : public Actor
{
public:
	SpaceStation(std::shared_ptr<Program> shaderProgram, EditorState& editorState, const std::string& name) :
		m_shaderProgram(shaderProgram),
		Actor(editorState, name)
	{
	}

	void init(const std::string& resourceDirectory) override;
	void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime) override;

private:
	std::shared_ptr<Shape> m_shape;
	std::shared_ptr<Program> m_shaderProgram;

	GLuint m_albedoTextureId;
};

