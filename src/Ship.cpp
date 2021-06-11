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

	auto mtlDir = resourceDirectory + "/Scene5/Ship/";
	m_shape->loadMesh(resourceDirectory + "/Scene5/Ship/Ship.obj", &mtlDir, stbi_load);

	//m_shape->resize();
	m_shape->init();

    setBaseRotation(glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)));

    setInterpolationMode(InterpolationMode::LINEAR);

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
    glUniform3f(m_shaderProgram->getUniform("albedo"), 2.0f, 2.0f, 2.0f);
    glUniform1f(m_shaderProgram->getUniform("roughness"), 0.05f);
    glUniform1f(m_shaderProgram->getUniform("metallic"), 1.0f);
    glUniform1f(m_shaderProgram->getUniform("ao"), 0.25f);
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
        Keyframe({2961.0000, -306.3485}, {2894.3333, -306.3485}, {3027.6667, -306.3485}),
        Keyframe({3161.0000, -308.6733}, {3094.3333, -308.6733}, {3227.6667, -308.6733}),
	});

	addPositionKeyframes(1, {
        Keyframe({2961.0000, 40.3574}, {2894.3333, 40.3574}, {3027.6667, 40.3574}),
        Keyframe({3161.0000, 23.6766}, {3094.3333, 23.6766}, {3227.6667, 23.6766}),
	});

	addPositionKeyframes(2, {
        Keyframe({2961.0000, 54.9331}, {2894.3333, 54.9331}, {3027.6667, 54.9331}),
        Keyframe({3161.0000, 56.8328}, {3094.3333, 56.8328}, {3227.6667, 56.8328}),
	});

	addRotationKeyframes(0, {
        Keyframe({2961.0000, 0.2249}, {2894.3333, 0.2249}, {3027.6667, 0.2249}),
        Keyframe({3161.0000, 0.2249}, {3094.3333, 0.2249}, {3227.6667, 0.2249}),
	});

	addRotationKeyframes(1, {
        Keyframe({2961.0000, -0.1445}, {2894.3333, -0.1445}, {3027.6667, -0.1445}),
        Keyframe({3161.0000, -0.1445}, {3094.3333, -0.1445}, {3227.6667, -0.1445}),
	});

	addRotationKeyframes(2, {
        Keyframe({2961.0000, 0.1006}, {2894.3333, 0.1006}, {3027.6667, 0.1006}),
        Keyframe({3161.0000, 0.1006}, {3094.3333, 0.1006}, {3227.6667, 0.1006}),
	});

	addRotationKeyframes(3, {
        Keyframe({2961.0000, 0.9583}, {2894.3333, 0.9583}, {3027.6667, 0.9583}),
        Keyframe({3161.0000, 0.9583}, {3094.3333, 0.9583}, {3227.6667, 0.9583}),
	});
}
