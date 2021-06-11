#pragma once

#include <memory>

#include "Actor.h"
#include "Shape.h"
#include "Program.h"
#include "SmartTexture.h"

class Sun : public Actor
{
public:
	Sun(std::shared_ptr<Shape> shape, EditorState& editorState, const std::string& name) :
		Actor(editorState, name),
		m_shape(shape)
	{
	}

	void init(const std::string& resourceDirectory);
	void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime);

private:
	std::shared_ptr<Shape> m_shape;
	std::shared_ptr<Program> m_shaderProgram;

	GLuint m_textureId;
};

