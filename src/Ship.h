#pragma once

#include <memory>

#include "AnimatedActor.h"
#include "Shape.h"
#include "Program.h"
#include "SmartTexture.h"

class Ship : public AnimatedActor
{
public:
	Ship(std::shared_ptr<Program> shaderProgram, GLuint skyboxTextureId, EditorState& editorState, const std::string& name) :
		m_shaderProgram(shaderProgram),
		m_skyboxTextureId(skyboxTextureId),
		AnimatedActor(editorState, name)
	{
	}

	void init(const std::string& resourceDirectory) override;
	void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, const glm::mat4& mMatrix) override;

private:
	GLuint m_skyboxTextureId;

	std::shared_ptr<Shape> m_shape;
	std::shared_ptr<Shape> m_tetherShape;
	std::shared_ptr<Program> m_shaderProgram;

	void addKeyframes();
};

