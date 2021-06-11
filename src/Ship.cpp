#include "Ship.h"

#include <iostream>

#include "stb_image.h"

void Ship::init(const std::string& resourceDirectory)
{
	AnimatedActor::init(resourceDirectory);

	m_shape = std::make_shared<Shape>();

    m_albedoTexture = SmartTexture::createTexture(resourceDirectory + "Materials/PaintedMetal/albedo.jpg");
    m_roughnessTexture = SmartTexture::createTexture(resourceDirectory + "Materials/PaintedMetal/roughness.jpg");
    m_metallicTexture = SmartTexture::createTexture(resourceDirectory + "Materials/PaintedMetal/metallic.jpg");
    m_aoTexture = SmartTexture::createTexture(resourceDirectory + "Materials/PaintedMetal/ao.jpg");

	auto mtlDir = resourceDirectory + "/Scene1/Ship/";
	m_shape->loadMesh(resourceDirectory + "/Scene1/Ship/Ship.obj", &mtlDir, stbi_load);

	//m_shape->resize();
	m_shape->init();

    setBaseRotation(glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)));

    addKeyframes();
}

void Ship::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, const glm::mat4& mMatrix)
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

	glUniformMatrix4fv(m_shaderProgram->getUniform("P"), 1, GL_FALSE, &pMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("V"), 1, GL_FALSE, &vMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("M"), 1, GL_FALSE, &mMatrix[0][0]);
    glUniform3fv(m_shaderProgram->getUniform("lightPositions"), LIGHT_COUNT, &editorState.lightPositions[0][0]);
    glUniform3fv(m_shaderProgram->getUniform("lightColors"), LIGHT_COUNT, &editorState.lightColors[0][0]);
	glUniform3fv(m_shaderProgram->getUniform("viewPos"), 1, &cameraPosition[0]);
    glUniform1i(m_shaderProgram->getUniform("useAlbedoMap"), 0);
    glUniform3f(m_shaderProgram->getUniform("albedo"), 2.0f, 2.0f, 2.0f);
    glUniform1f(m_shaderProgram->getUniform("roughness"), 0.05f);
    glUniform1f(m_shaderProgram->getUniform("metallic"), 1.0f);
    glUniform1f(m_shaderProgram->getUniform("ao"), 0.02f);
	m_shape->draw(m_shaderProgram);			//render!!!!!!!
	m_shaderProgram->unbind();

    m_albedoTexture->unbind();
    m_roughnessTexture->unbind();
    m_metallicTexture->unbind();
    m_aoTexture->unbind();
}

void Ship::addKeyframes()
{
	addPositionKeyframes(0, {
        Keyframe({374.0000, 2.5851}, {373.0000, 2.5851}, {375.0000, 2.5851}),
	});

	addPositionKeyframes(1, {
        Keyframe({374.0000, -3.5971}, {373.0000, -3.5971}, {375.0000, -3.5971}),
	});

	addPositionKeyframes(2, {
        Keyframe({374.0000, 3.6427}, {373.0000, 3.6427}, {375.0000, 3.6427}),
	});

	addRotationKeyframes(0, {
        Keyframe({374.0000, 0.2643}, {373.0000, 0.2643}, {375.0000, 0.2643}),
	});
    
	addRotationKeyframes(1, {
        Keyframe({374.0000, -0.3877}, {373.0000, -0.3877}, {375.0000, -0.3877}),
	});

	addRotationKeyframes(2, {
        Keyframe({374.0000, 0.8560}, {373.0000, 0.8560}, {375.0000, 0.8560}),
	});

	addRotationKeyframes(3, {
        Keyframe({374.0000, -0.2169}, {373.0000, -0.2169}, {375.0000, -0.2169}),
	});
}
