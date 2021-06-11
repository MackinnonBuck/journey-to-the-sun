#include "Clip.h"

Clip::Clip(const std::vector<glm::vec3>& controlPoints)
{
	m_controlPointLine.init();
	m_splinePointLine.init();

	if (controlPoints.size() == 2)
	{
		for (int i = 0; i < SPLINE_RESOLUTION; i++)
		{
			m_splinePoints.push_back(glm::mix(
				controlPoints[0],
				controlPoints[1],
				i / (float)(SPLINE_RESOLUTION - 1)));
		}
	}
	else if (controlPoints.size() > 2)
	{
		spline(m_splinePoints, controlPoints, SPLINE_RESOLUTION);
	}

	m_controlPointLine.re_init_line(controlPoints);
	m_splinePointLine.re_init_line(m_splinePoints);
}

void Clip::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix)
{
	m_controlPointLine.draw(pMatrix, vMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
	m_splinePointLine.draw(pMatrix, vMatrix, glm::vec3(0.0f, 1.0f, 1.0f));
}
