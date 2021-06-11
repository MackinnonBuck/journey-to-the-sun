#include "AnimatedActor.h"

#include <iostream>

void AnimatedActor::init(const std::string& resourceDirectory)
{
}

void AnimatedActor::update(float deltaTime)
{
	auto& editorState = getEditorState();
    auto frameNumber = editorState.totalTime * 30.0f; // 30 frames per second. TODO: Don't hardcode pls? :)

	for (size_t i = 0; i < 3; i++)
	{
		getKeyframeValue(m_positionKeyframes[i], frameNumber, m_position[i]);
	}

	for (size_t i = 0; i < 3; i++)
	{
		getKeyframeValue(m_scaleKeyframes[i], frameNumber, m_scale[i]);
	}

	for (size_t i = 0; i < 4; i++)
	{
        // We rotate the values here to convert from WXYZ order ot XYZW.
		getKeyframeValue(m_rotationKeyframes[(i + 1) % 4], frameNumber, m_rotation[i]);
	}
}

glm::mat4 AnimatedActor::computeMatrix() const
{
	return
		glm::translate(glm::mat4(1.0f), m_position) *
		glm::mat4(m_rotation) *
		glm::mat4(m_baseRotation) *
		glm::scale(glm::mat4(1.0f), m_scale);
}

void AnimatedActor::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime)
{
	render(pMatrix, vMatrix, computeMatrix());
}

void AnimatedActor::addPositionKeyframes(int component, const std::vector<Keyframe>& keyframes)
{
    assert(component < 3);

    addKeyframes(m_positionKeyframes[component], keyframes);
}

void AnimatedActor::addScaleKeyframes(int component, const std::vector<Keyframe>& keyframes)
{
    assert(component < 3);

    addKeyframes(m_scaleKeyframes[component], keyframes);
}

void AnimatedActor::addRotationKeyframes(int component, const std::vector<Keyframe>& keyframes)
{
    assert(component < 4);

    addKeyframes(m_rotationKeyframes[component], keyframes);
}

void AnimatedActor::addKeyframes(KeyframeMap& keyframeMap, const std::vector<Keyframe>& keyframes)
{
    for (auto& keyframe : keyframes)
    {
        keyframeMap[keyframe.getControlPoint().x] = keyframe;
    }
}

bool AnimatedActor::getKeyframeValue(const KeyframeMap& keyframeMap, float totalTime, float& outValue)
{
	auto rightIt = keyframeMap.lower_bound(totalTime);

	if (rightIt == keyframeMap.end())
	{
		return false;
	}

	auto& rightKeyframe = rightIt->second;

	auto leftIt = std::prev(rightIt);

	if (leftIt == keyframeMap.end())
	{
		outValue = rightKeyframe.getControlPoint().y;
		return true;
	}

	auto& leftKeyframe = leftIt->second;

	outValue = Keyframe::getValue(m_interpolationMode, leftKeyframe, rightKeyframe, totalTime);
	return true;
}
