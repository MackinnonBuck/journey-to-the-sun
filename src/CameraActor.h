#pragma once

#include "AnimatedActor.h"

class CameraActor : public AnimatedActor
{
public:
	CameraActor(const AnimatedActor& target, EditorState& editorState, const std::string& name) :
		AnimatedActor(editorState, name),
		m_target(target),
		m_transform(glm::mat4(1.0f))
	{
	}

	void init(const std::string& resourceDirectory) override;
	void update(float deltaTime) override;

	glm::mat4 computeMatrix() const override;

protected:
	//glm::mat4 computeMatrix(const glm::mat4& translationMatrix, const glm::mat4& rotationMatrix) const override;
	//glm::vec3 getRotationDirection() const override;
	//glm::vec3 getFlyDirection() const override;

private:
	const AnimatedActor& m_target;
	glm::mat4 m_transform;

	void addKeyframes();
};

