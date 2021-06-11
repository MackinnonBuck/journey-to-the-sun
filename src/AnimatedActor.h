#pragma once

#include <map>
#include <memory>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "Actor.h"
#include "Keyframe.h"
#include "InterpolationMode.h"

class AnimatedActor : public Actor
{
public:
	AnimatedActor(EditorState& editorState, const std::string& name) :
		Actor(editorState, name),
		m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
		m_rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		m_baseRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		m_interpolationMode(InterpolationMode::BEZIER)
	{
	}

	void init(const std::string& resourceDirectory) override;
	void update(float deltaTime) override;
	void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime) final;

	glm::vec3 getPosition() const { return m_position; }
	glm::vec3 getScale() const { return m_scale; }
	glm::quat getRotation() const { return m_rotation; }

	virtual glm::mat4 computeMatrix() const;

protected:
	void setBaseRotation(const glm::quat& baseRotation) { m_baseRotation = baseRotation; }
	void setInterpolationMode(InterpolationMode interpolationMode) { m_interpolationMode = interpolationMode; }

	void addPositionKeyframes(int component, const std::vector<Keyframe>& keyframes);
	void addScaleKeyframes(int component, const std::vector<Keyframe>& keyframes);
	void addRotationKeyframes(int component, const std::vector<Keyframe>& keyframes);

	virtual void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, const glm::mat4& mMatrix) { };

private:
	using KeyframeMap = std::map<float, Keyframe>;

	KeyframeMap m_positionKeyframes[3];
	KeyframeMap m_scaleKeyframes[3];
	KeyframeMap m_rotationKeyframes[4];

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::quat m_rotation;
	glm::quat m_baseRotation;

	InterpolationMode m_interpolationMode;

	void addKeyframes(KeyframeMap& keyframeMap, const std::vector<Keyframe>& keyframes);
	bool getKeyframeValue(const KeyframeMap& keyframeMap, float totalTime, float& outValue);
};

