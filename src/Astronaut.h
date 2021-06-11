#pragma once

#include "AnimatedActor.h"
#include "Program.h"
#include "Shape.h"

class Astronaut : public AnimatedActor
{
public:
	Astronaut(std::shared_ptr<Program> shaderProgram, EditorState& editorState, const std::string& name) :
		AnimatedActor(editorState, name),
		m_shaderProgram(shaderProgram)
	{
	}

	void init(const std::string& resourceDirectory) override;
	void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, const glm::mat4& mMatrix) override;

private:
	std::shared_ptr<Shape> m_shape;
	std::shared_ptr<Program> m_shaderProgram;

	void addKeyframes();
};

