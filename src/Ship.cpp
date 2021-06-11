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

	auto mtlDir = resourceDirectory + "/Scene3/Ship/";
	m_shape->loadMesh(resourceDirectory + "/Scene3/Ship/Ship.obj", &mtlDir, stbi_load);

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
        Keyframe({1979.0000, -292.5153}, {1815.6666, -292.5153}, {2142.3333, -292.5153}),
        Keyframe({2469.0000, -308.0543}, {2305.6667, -308.0543}, {2632.3333, -308.0543}),
	});
    
	addPositionKeyframes(1, {
        Keyframe({1979.0000, 42.0697}, {1815.6666, 42.0697}, {2142.3333, 42.0697}),
        Keyframe({2469.0000, 30.2410}, {2305.6667, 30.2410}, {2632.3333, 30.2410}),
	});

	addPositionKeyframes(2, {
        Keyframe({1979.0000, 51.4929}, {1815.6666, 51.4929}, {2142.3333, 51.4929}),
        Keyframe({2469.0000, 56.9790}, {2305.6667, 56.9790}, {2632.3333, 56.9790}),
	});

	addRotationKeyframes(0, {
        Keyframe({1979.0000, 0.5959}, {1815.6666, 0.5959}, {2142.3333, 0.5959}),
        Keyframe({2469.0000, 0.2249}, {2305.6667, 0.2249}, {2632.3333, 0.2249}),
	});

	addRotationKeyframes(1, {
			Keyframe({1979.0000, -0.0056}, {1815.6666, -0.0056}, {2142.3333, -0.0056}),
        Keyframe({2469.0000, -0.1445}, {2305.6667, -0.1445}, {2632.3333, -0.1445}),
	});

	addRotationKeyframes(2,{
        Keyframe({1979.0000, 0.1981}, {1815.6666, 0.1981}, {2142.3333, 0.1981}),
        Keyframe({2469.0000, 0.1006}, {2305.6667, 0.1006}, {2632.3333, 0.1006}),
	});

	addRotationKeyframes(3, {
        Keyframe({1979.0000, 0.7782}, {1815.6666, 0.7782}, {2142.3333, 0.7782}),
        Keyframe({2469.0000, 0.9583}, {2305.6667, 0.9583}, {2632.3333, 0.9583}),
	});
}
