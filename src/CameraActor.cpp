#include "CameraActor.h"

// TODO: Everything mostly works for non-camera actors, now it needs to work for the camera.

//glm::mat4 CameraActor::computeMatrix(const glm::mat4& translationMatrix, const glm::mat4& rotationMatrix) const
//{
//	return rotationMatrix * translationMatrix;
//}

void CameraActor::init(const std::string& resourceDirectory)
{
    setInterpolationMode(InterpolationMode::LINEAR);
	addKeyframes();
}

glm::mat4 CameraActor::computeMatrix() const
{
    return glm::inverse(AnimatedActor::computeMatrix());
}

void CameraActor::update(float deltaTime)
{
	AnimatedActor::update(deltaTime);

	auto& editorState = getEditorState();
	editorState.cameraPosition = -getPosition();

    //setBaseRotation(glm::quat(glm::inverse(glm::lookAt(getPosition(), m_target.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f)))));
}

void CameraActor::addKeyframes()
{
	addPositionKeyframes(0, {
        Keyframe({2961.0000, -323.6096}, {2880.0000, -323.6096}, {3042.0000, -323.6096}),
        Keyframe({3204.0000, -323.6096}, {3123.0000, -323.6096}, {3285.0000, -323.6096}),
	});

	addPositionKeyframes(1, {
        Keyframe({2961.0000, 43.0499}, {2880.0000, 43.0499}, {3042.0000, 43.0499}),
        Keyframe({3204.0000, 43.0499}, {3123.0000, 43.0499}, {3285.0000, 43.0499}),
	});

	addPositionKeyframes(2, {
        Keyframe({2961.0000, 62.9949}, {2880.0000, 62.9949}, {3042.0000, 62.9949}),
        Keyframe({3204.0000, 62.9949}, {3123.0000, 62.9949}, {3285.0000, 62.9949}),
	});

	addRotationKeyframes(0, {
        Keyframe({2961.0000, 0.4158}, {2880.0000, 0.4158}, {3042.0000, 0.4158}),
        Keyframe({3204.0000, 0.4763}, {3123.0000, 0.4763}, {3285.0000, 0.4763}),
	});

	addRotationKeyframes(1, {
        Keyframe({2961.0000, 0.2595}, {2880.0000, 0.2595}, {3042.0000, 0.2595}),
        Keyframe({3204.0000, 0.3901}, {3123.0000, 0.3901}, {3285.0000, 0.3901}),
	});
	
	addRotationKeyframes(2, {
        Keyframe({2961.0000, -0.4614}, {2880.0000, -0.4614}, {3042.0000, -0.4614}),
        Keyframe({3204.0000, -0.4993}, {3123.0000, -0.4993}, {3285.0000, -0.4993}),
	});

	addRotationKeyframes(3, {
        Keyframe({2961.0000, -0.7395}, {2880.0000, -0.7395}, {3042.0000, -0.7395}),
        Keyframe({3204.0000, -0.6097}, {3123.0000, -0.6097}, {3285.0000, -0.6097}),
	});
}

//glm::vec3 CameraActor::getRotationDirection() const
//{
//	return -glm::vec3(getPitchSpeed(), getYawSpeed(), getRollSpeed());
//}
//
//glm::vec3 CameraActor::getFlyDirection() const
//{
//	return glm::vec4(0, 0, 1, 1) * computeRotationMatrix(getRotation());
//}
