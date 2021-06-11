#include "Asteroid.h"

#include <iostream>

#include "stb_image.h"

void Asteroid::init(const std::string& resourceDirectory)
{
	Actor::init(resourceDirectory);
}

void Asteroid::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime)
{
	auto& editorState = getEditorState();

	glm::mat4 mMatrix =
		glm::translate(glm::mat4(1.0f), m_position) *
		glm::rotate(glm::mat4(1.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::vec3 cameraPosition = -editorState.cameraPosition;

	m_shaderProgram->bind();
	glUniformMatrix4fv(m_shaderProgram->getUniform("P"), 1, GL_FALSE, &pMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("V"), 1, GL_FALSE, &vMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("M"), 1, GL_FALSE, &mMatrix[0][0]);
    //glUniform3fv(m_shaderProgram->getUniform("lightPosition"), 1, &lightPosition[0]);
    //glUniform3fv(m_shaderProgram->getUniform("lightColor"), 1, &lightColor[0]);
    glUniform3fv(m_shaderProgram->getUniform("lightPositions"), LIGHT_COUNT, &editorState.lightPositions[0][0]);
    glUniform3fv(m_shaderProgram->getUniform("lightColors"), LIGHT_COUNT, &editorState.lightColors[0][0]);
	glUniform3fv(m_shaderProgram->getUniform("viewPos"), 1, &cameraPosition[0]);
    glUniform3f(m_shaderProgram->getUniform("albedo"), 1.0f, 1.0f, 1.0f);
    glUniform1f(m_shaderProgram->getUniform("roughness"), 0.8f);
    glUniform1f(m_shaderProgram->getUniform("metallic"), 0.1f);
    glUniform1f(m_shaderProgram->getUniform("ao"), 0.25f);

	m_shape->draw(m_shaderProgram);			//render!!!!!!!
	m_shaderProgram->unbind();

}
