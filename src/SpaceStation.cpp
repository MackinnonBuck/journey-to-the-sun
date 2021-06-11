#include "SpaceStation.h"

#include <iostream>

#include "stb_image.h"

void SpaceStation::init(const std::string& resourceDirectory)
{
	m_shape = std::make_shared<Shape>();

	auto mtlDir = resourceDirectory + "/Scene1/SpaceStation/";
	m_shape->loadMesh(resourceDirectory + "/Scene1/SpaceStation/SpaceStation.obj", &mtlDir, stbi_load);

	//m_shape->resize();
	m_shape->init();

    //setBaseRotation(glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)));
}

void SpaceStation::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime)
{
	auto& editorState = getEditorState();

    //glm::vec3 lightPosition = glm::vec3(0.0f, 1000.0f, 0.0f);
    //glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 0.7f) * 500000.0f;

	m_shaderProgram->bind();

    //m_albedoTexture->bind(0, 1);
    //m_roughnessTexture->bind(0, 2);
    //m_metallicTexture->bind(0, 3);
    //m_aoTexture->bind(0, 4);
    
    // TODO: Better solution for skybox.
 //   glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_skyboxTextureId);

    glm::vec3 cameraPosition = -editorState.cameraPosition;

    auto mMatrix =
        glm::mat4(glm::quat(0.677f, 0.078f, 0.432f, -0.591f)) *
        glm::rotate(glm::mat4(1.0f), glm::pi<float>() * 2 * (editorState.totalTime / (991.0f / 30.0f)), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(m_shaderProgram->getUniform("P"), 1, GL_FALSE, &pMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("V"), 1, GL_FALSE, &vMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("M"), 1, GL_FALSE, &mMatrix[0][0]);
    glUniform3fv(m_shaderProgram->getUniform("lightPositions"), LIGHT_COUNT, &editorState.lightPositions[0][0]);
    glUniform3fv(m_shaderProgram->getUniform("lightColors"), LIGHT_COUNT, &editorState.lightColors[0][0]);
	glUniform3fv(m_shaderProgram->getUniform("viewPos"), 1, &cameraPosition[0]);
    glUniform3f(m_shaderProgram->getUniform("albedo"), 2.0f, 2.0f, 2.0f);
    glUniform1f(m_shaderProgram->getUniform("roughness"), 0.0f);
    glUniform1f(m_shaderProgram->getUniform("metallic"), 0.5f);
    glUniform1f(m_shaderProgram->getUniform("ao"), 0.015f);
	m_shape->draw(m_shaderProgram);			//render!!!!!!!
	m_shaderProgram->unbind();
}
