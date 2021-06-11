#pragma once

#include "AnimatedActor.h"

class CameraActor : public AnimatedActor
{
public:
	CameraActor(const AnimatedActor& target, EditorState& editorState, const std::string& name) :
		AnimatedActor(editorState, name),
		m_target(target)
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
	glm::vec3 m_position = glm::vec3(3.6779f, -8.0276f, 0.3533f);
	//glm::vec3 m_position = glm::vec3(3.6779f, -18.0276f, 0.3533f);

	void addKeyframes();
};

