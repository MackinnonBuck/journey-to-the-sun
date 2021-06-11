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
	// TODO
 //   addPositionKeyframes(0, {
 //       Keyframe({1.0000, 18.0870}, {-82.0000, 18.0870}, {84.0000, 18.0870}),
 //       Keyframe({250.0000, 5.0044}, {167.0000, 6.8451}, {497.3333, -0.4808}),
 //       Keyframe({992.0000, -0.4808}, {744.6667, -0.4808}, {1320.6666, -0.4808}),
 //       Keyframe({1978.0000, 56.8935}, {1649.3334, 56.8935}, {2306.6667, 56.8935}),
	//});

	//addPositionKeyframes(1, {
 //       Keyframe({1.0000, -70.1714}, {-82.0000, -70.1714}, {84.0000, -70.1714}),
 //       Keyframe({250.0000, -8.8832}, {167.0000, -8.8832}, {497.3333, -8.8832}),
 //       Keyframe({992.0000, -45.0076}, {744.6667, -45.0076}, {1320.6666, -45.0076}),
 //       Keyframe({1978.0000, 689.4220}, {1649.3334, 689.4220}, {2306.6667, 689.4220}),
	//});

 //   addPositionKeyframes(2, {
 //       Keyframe({1.0000, 7.6400}, {-82.0000, 7.6400}, {84.0000, 7.6400}),
 //       Keyframe({250.0000, 0.9699}, {167.0000, 1.0823}, {497.3333, 0.6348}),
 //       Keyframe({992.0000, 0.6348}, {744.6667, 0.6348}, {1320.6666, 0.6348}),
 //       Keyframe({1978.0000, 2.0911}, {1649.3334, 2.0911}, {2306.6667, 2.0911}),
	//});

	addPositionKeyframes(0, {
        Keyframe({1979.0000, -323.6096}, {1815.6666, -323.6096}, {2142.3333, -323.6096}),
        Keyframe({2469.0000, -323.6096}, {2305.6667, -323.6096}, {2632.3333, -323.6096}),
	});

	addPositionKeyframes(1, {
			Keyframe({1979.0000, 43.0499}, {1815.6666, 43.0499}, {2142.3333, 43.0499}),
			Keyframe({2469.0000, 43.0499}, {2305.6667, 43.0499}, {2632.3333, 43.0499}),
	});

	addPositionKeyframes(2, {
			Keyframe({1979.0000, 59.3496}, {1815.6666, 59.3496}, {2142.3333, 59.3496}),
			Keyframe({2469.0000, 59.3496}, {2305.6667, 59.3496}, {2632.3333, 59.3496}),
	});

    addRotationKeyframes(0, {
        Keyframe({1979.0000, 0.4270}, {1815.6666, 0.4270}, {2142.3333, 0.4270}),
        Keyframe({2469.0000, 0.5257}, {2305.6667, 0.5257}, {2632.3333, 0.5257}),
	});

	addRotationKeyframes(1, {
        Keyframe({1979.0000, 0.3244}, {1815.6666, 0.3244}, {2142.3333, 0.3244}),
        Keyframe({2469.0000, 0.4650}, {2305.6667, 0.4650}, {2632.3333, 0.4650}),
	});
	
	addRotationKeyframes(2, {
        Keyframe({1979.0000, -0.5105}, {1815.6666, -0.5105}, {2142.3333, -0.5105}),
        Keyframe({2469.0000, -0.4719}, {2305.6667, -0.4719}, {2632.3333, -0.4719}),
	});

	addRotationKeyframes(3, {
        Keyframe({1979.0000, -0.6722}, {1815.6666, -0.6722}, {2142.3333, -0.6722}),
        Keyframe({2469.0000, -0.5336}, {2305.6667, -0.5336}, {2632.3333, -0.5336}),
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
