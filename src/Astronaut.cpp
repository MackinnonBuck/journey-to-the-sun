#include "Astronaut.h"

#include "stb_image.h"

void Astronaut::init(const std::string& resourceDirectory)
{
	m_shape = std::make_shared<Shape>();

	auto mtlDir = resourceDirectory + "Scene7/Astronaut/";
	m_shape->loadMesh(resourceDirectory + "Scene7/Astronaut/Astronaut.obj", &mtlDir, stbi_load);

	//m_shape->resize();
	m_shape->init();

    setBaseRotation(glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)));

	addKeyframes();
}

void Astronaut::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, const glm::mat4& mMatrix)
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

	m_shaderProgram->unbind();
}

void Astronaut::addKeyframes()
{
	addPositionKeyframes(0, {
		        Keyframe({4683.0000, 3.6127}, {4601.0000, 3.6127}, {4765.0000, 3.6127}),
        Keyframe({4929.0000, 3.6127}, {4847.0000, 3.6127}, {5011.0000, 3.6127}),
	});

	addPositionKeyframes(1, {
		Keyframe({4683.0000, -5.8663}, {4601.0000, -5.8663}, {4765.0000, -5.8663}),
        Keyframe({4929.0000, -5.8663}, {4847.0000, -5.8663}, {5011.0000, -5.8663}),
	});

	addPositionKeyframes(2, {
		Keyframe({4683.0000, 0.5194}, {4601.0000, 0.5194}, {4765.0000, 0.5194}),
        Keyframe({4929.0000, 0.5194}, {4847.0000, 0.5194}, {5011.0000, 0.5194}),
	});

	addRotationKeyframes(0, {
		Keyframe({4683.0000, 0.9917}, {4601.0000, 0.9917}, {4765.0000, 0.9917}),
        Keyframe({4929.0000, 0.7441}, {4847.0000, 0.7441}, {5011.0000, 0.7441}),
	});

	addRotationKeyframes(1, {
		Keyframe({4683.0000, -0.0766}, {4601.0000, -0.0766}, {4765.0000, -0.0766}),
        Keyframe({4929.0000, -0.2349}, {4847.0000, -0.2349}, {5011.0000, -0.2349}),
	});
	
	addRotationKeyframes(2, {
		Keyframe({4683.0000, -0.1001}, {4601.0000, -0.1001}, {4765.0000, -0.1001}),
        Keyframe({4929.0000, -0.3567}, {4847.0000, -0.3567}, {5011.0000, -0.3567}),
	});

	addRotationKeyframes(3, {
		Keyframe({4683.0000, -0.0272}, {4601.0000, -0.0272}, {4765.0000, -0.0272}),
        Keyframe({4929.0000, 0.5137}, {4847.0000, 0.5137}, {5011.0000, 0.5137}),
	});
}
