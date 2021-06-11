#pragma once

#include "AnimatedActor.h"
#include "SpaceStation.h"

class CameraActor : public Actor
{
public:
	CameraActor(const SpaceStation& target, EditorState& editorState, const std::string& name) :
		Actor(editorState, name),
		m_target(target),
		m_transform(glm::mat4(1.0f)),
		m_position(glm::vec3(0.0f))
	{
	}

	void init(const std::string& resourceDirectory) override;
	void update(float deltaTime) override;

	glm::mat4 computeMatrix();

protected:
	//glm::mat4 computeMatrix(const glm::mat4& translationMatrix, const glm::mat4& rotationMatrix) const override;
	//glm::vec3 getRotationDirection() const override;
	//glm::vec3 getFlyDirection() const override;

private:
	const SpaceStation& m_target;
	glm::mat4 m_transform;
	glm::vec3 m_position;

	//void addKeyframes();
};

