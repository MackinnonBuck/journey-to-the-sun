#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "line.h"

class Clip
{
public:
	static const int SPLINE_RESOLUTION = 30;

	Clip(const std::vector<glm::vec3>& controlPoints);
	
	const std::vector<glm::vec3>& getSplinePoints() const { return m_splinePoints; }

	void render(const glm::mat4& pMatrix, const glm::mat4& vMatrix);

private:
	std::vector<glm::vec3> m_splinePoints;

	Line m_controlPointLine;
	Line m_splinePointLine;
};

