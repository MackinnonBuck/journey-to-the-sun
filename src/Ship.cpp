#include "Ship.h"

#include <iostream>

#include "stb_image.h"

void Ship::init(const std::string& resourceDirectory)
{
	AnimatedActor::init(resourceDirectory);

	m_shape = std::make_shared<Shape>();

	auto mtlDir = resourceDirectory + "Scene7/Ship/";
	m_shape->loadMesh(resourceDirectory + "Scene7/Ship/Ship.obj", &mtlDir, stbi_load);

	//m_shape->resize();
	m_shape->init();

    m_tetherShape = std::make_shared<Shape>();

    auto tetherMtlDir = resourceDirectory + "Scene7/Tether/";
	m_tetherShape->loadMesh(resourceDirectory + "Scene7/Tether/Tether.obj", &tetherMtlDir, stbi_load);
	m_tetherShape->init();

 //   auto tetherMtlDir = resourceDirectory + "Scene7/Ship/";
	//m_tetherShape->loadMesh(resourceDirectory + "Scene7/Ship/Ship.obj", &tetherMtlDir, stbi_load);

    setBaseRotation(glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)));

    addKeyframes();

    setInterpolationMode(InterpolationMode::LINEAR);
}

void Ship::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, const glm::mat4& mMatrix)
{
	auto& editorState = getEditorState();

	m_shaderProgram->bind();

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
    glUniform1f(m_shaderProgram->getUniform("ao"), 0.0f);
	m_shape->draw(m_shaderProgram);			//render!!!!!!!

	auto tetherTransform = glm::rotate(glm::mat4(1.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(m_shaderProgram->getUniform("M"), 1, GL_FALSE, &tetherTransform[0][0]);

	m_tetherShape->draw(m_shaderProgram);

	m_shaderProgram->unbind();
}

void Ship::addKeyframes()
{
    addPositionKeyframes(0, {
        Keyframe({4683.0000, 0.3740}, {4601.0000, 0.3740}, {4765.0000, 0.3740}),
        Keyframe({4929.0000, 0.3740}, {4847.0000, 0.3740}, {5011.0000, 0.3740}),
	});

	addPositionKeyframes(1, {
        Keyframe({4683.0000, 4.0282}, {4601.0000, 4.0282}, {4765.0000, 4.0282}),
        Keyframe({4929.0000, 4.0282}, {4847.0000, 4.0282}, {5011.0000, 4.0282}),
	});

	addPositionKeyframes(2, {
        Keyframe({4683.0000, 2.8964}, {4601.0000, 2.8964}, {4765.0000, 2.8964}),
        Keyframe({4929.0000, 2.8964}, {4847.0000, 2.8964}, {5011.0000, 2.8964}),
	});

	addRotationKeyframes(0, {
        Keyframe({4683.0000, -0.6635}, {4601.0000, -0.6635}, {4765.0000, -0.6635}),
        Keyframe({4929.0000, -0.7777}, {4847.0000, -0.7777}, {5011.0000, -0.7777}),
	});

	addRotationKeyframes(1, {
        Keyframe({4683.0000, -0.6659}, {4601.0000, -0.6659}, {4765.0000, -0.6659}),
        Keyframe({4929.0000, -0.4955}, {4847.0000, -0.4955}, {5011.0000, -0.4955}),
	});

	addRotationKeyframes(2, {
        Keyframe({4683.0000, -0.0360}, {4601.0000, -0.0360}, {4765.0000, -0.0360}),
        Keyframe({4929.0000, -0.3722}, {4847.0000, -0.3722}, {5011.0000, -0.3722}),
	});

	addRotationKeyframes(3, {
        Keyframe({4683.0000, 0.3392}, {4601.0000, 0.3392}, {4765.0000, 0.3392}),
        Keyframe({4929.0000, -0.1060}, {4847.0000, -0.1060}, {5011.0000, -0.1060}),
	});
}
