#pragma once

#include <glm/gtc/type_ptr.hpp>

constexpr size_t LIGHT_COUNT = 5;

struct EditorState
{
	float totalTime;
	float playbackSpeed;
	int selectedActorIndex;
	glm::vec3 cameraPosition;
	bool debuggingEnabled;

	glm::vec3 lightPositions[LIGHT_COUNT];
	glm::vec3 lightColors[LIGHT_COUNT];
};
