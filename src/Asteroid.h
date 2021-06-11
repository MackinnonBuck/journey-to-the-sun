#pragma once

#include <memory>

#include "Actor.h"
#include "Shape.h"
#include "Program.h"

class Asteroid : public Actor
{
public:
	Asteroid(
		const glm::vec3& position,
		std::shared_ptr<Shape> shape,
		std::shared_ptr<Program> shaderProgram,
		EditorState& editorState,
		const std::string& name) :
		Actor(editorState, name),
		m_position(position),
		m_shape(shape),
		m_shaderProgram(shaderProgram)
	{
	}

	void init(const std::string& resourceDirectory);
	void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime);

private:
	std::shared_ptr<Shape> m_shape;
	std::shared_ptr<Program> m_shaderProgram;

	glm::vec3 m_position;
};

