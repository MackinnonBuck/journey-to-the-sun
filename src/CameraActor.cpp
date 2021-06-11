#include "CameraActor.h"

// TODO: Everything mostly works for non-camera actors, now it needs to work for the camera.

//glm::mat4 CameraActor::computeMatrix(const glm::mat4& translationMatrix, const glm::mat4& rotationMatrix) const
//{
//	return rotationMatrix * translationMatrix;
//}

void CameraActor::init(const std::string& resourceDirectory)
{
    //setInterpolationMode(InterpolationMode::LINEAR);
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
        Keyframe({498.0000, -5.9199}, {497.6667, -5.9199}, {498.3333, -5.9199}),
        Keyframe({499.0000, 11.6013}, {498.6667, 11.6013}, {499.3333, 11.6013}),
	});

	addPositionKeyframes(1, {
        Keyframe({498.0000, -47.1291}, {497.6667, -47.1291}, {498.3333, -47.1291}),
        Keyframe({499.0000, 4.9039}, {498.6667, 4.9039}, {499.3333, 4.9039}),
	});

	addPositionKeyframes(2, {
        Keyframe({498.0000, -1.4738}, {497.6667, -1.4738}, {498.3333, -1.4738}),
        Keyframe({499.0000, -4.4362}, {498.6667, -4.4362}, {499.3333, -4.4362}),
	});

	addRotationKeyframes(0, {
        Keyframe({1.0000, 0.1723}, {0.3333, 0.1723}, {1.6667, 0.1723}),
        Keyframe({3.0000, 0.1748}, {2.3333, 0.1735}, {47.3333, 0.2557}),
        Keyframe({136.0000, 0.5214}, {91.6667, 0.4124}, {176.6667, 0.6213}),
        Keyframe({258.0000, 0.6812}, {217.3333, 0.6812}, {268.0000, 0.6812}),
        Keyframe({288.0000, 0.6807}, {278.0000, 0.6807}, {357.6667, 0.6807}),
        Keyframe({497.0000, 0.7265}, {427.3333, 0.7265}, {497.3333, 0.7265}),
        Keyframe({498.0000, 0.7265}, {497.6667, 0.7265}, {498.3333, 0.7265}),
        Keyframe({499.0000, 0.2144}, {498.6667, 0.2144}, {499.3333, 0.2144}),
	});

	addRotationKeyframes(1, {
        Keyframe({1.0000, 0.5684}, {0.3333, 0.5684}, {1.6667, 0.5684}),
        Keyframe({3.0000, 0.5715}, {2.3333, 0.5700}, {47.3333, 0.6737}),
        Keyframe({136.0000, 0.7746}, {91.6667, 0.7746}, {176.6667, 0.7746}),
        Keyframe({258.0000, 0.7294}, {217.3333, 0.7294}, {268.0000, 0.7294}),
        Keyframe({288.0000, 0.7301}, {278.0000, 0.7301}, {357.6667, 0.7301}),
        Keyframe({497.0000, 0.6843}, {427.3333, 0.6843}, {497.3333, 0.6843}),
        Keyframe({498.0000, 0.6843}, {497.6667, 0.6843}, {498.3333, 0.6843}),
        Keyframe({499.0000, 0.3773}, {498.6667, 0.3773}, {499.3333, 0.3773}),
	});

	addRotationKeyframes(2, {
        Keyframe({1.0000, -0.7699}, {0.3333, -0.7699}, {1.6667, -0.7699}),
        Keyframe({3.0000, -0.7667}, {2.3333, -0.7683}, {47.3333, -0.6614}),
        Keyframe({136.0000, -0.2969}, {91.6667, -0.4534}, {176.6667, -0.1534}),
        Keyframe({258.0000, -0.0460}, {217.3333, -0.0533}, {268.0000, -0.0442}),
        Keyframe({288.0000, -0.0442}, {278.0000, -0.0442}, {357.6667, -0.0442}),
        Keyframe({497.0000, -0.0432}, {427.3333, -0.0432}, {497.3333, -0.0432}),
        Keyframe({498.0000, -0.0432}, {497.6667, -0.0432}, {498.3333, -0.0432}),
        Keyframe({499.0000, 0.7833}, {498.6667, 0.7833}, {499.3333, 0.7833}),
	});

	addRotationKeyframes(3, {
        Keyframe({1.0000, -0.2334}, {0.3333, -0.2334}, {1.6667, -0.2334}),
        Keyframe({3.0000, -0.2344}, {2.3333, -0.2344}, {47.3333, -0.2344}),
        Keyframe({136.0000, -0.1998}, {91.6667, -0.2344}, {176.6667, -0.1681}),
        Keyframe({258.0000, -0.0430}, {217.3333, -0.0500}, {268.0000, -0.0412}),
        Keyframe({288.0000, -0.0412}, {278.0000, -0.0412}, {357.6667, -0.0412}),
        Keyframe({497.0000, -0.0458}, {427.3333, -0.0458}, {497.3333, -0.0458}),
        Keyframe({498.0000, -0.0458}, {497.6667, -0.0458}, {498.3333, -0.0458}),
        Keyframe({499.0000, 0.4451}, {498.6667, 0.4451}, {499.3333, 0.4451}),
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
