#include "Sun.h"

#include <iostream>

void Sun::init(const std::string& resourceDirectory)
{
	m_shaderProgram = std::make_shared<Program>();
	m_shaderProgram->setVerbose(true);
	m_shaderProgram->setShaderNames(resourceDirectory + "/sun_vertex.glsl", resourceDirectory + "/sun_fragment.glsl");
	if (!m_shaderProgram->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	m_shaderProgram->addUniform("P");
	m_shaderProgram->addUniform("V");
	m_shaderProgram->addUniform("M");
	m_shaderProgram->addAttribute("vertPos");
	m_shaderProgram->addAttribute("vertNor");
	m_shaderProgram->addAttribute("vertTex");
}

void Sun::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime)
{
	glm::mat4 mMatrix =
		glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 1000.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(15.0f));

	m_shaderProgram->bind();
	glUniformMatrix4fv(m_shaderProgram->getUniform("P"), 1, GL_FALSE, &pMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("V"), 1, GL_FALSE, &vMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("M"), 1, GL_FALSE, &mMatrix[0][0]);

	m_shape->draw(m_shaderProgram);
	m_shaderProgram->unbind();
}
