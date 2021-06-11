#include "Astronaut.h"

#include "stb_image.h"

void Astronaut::init(const std::string& resourceDirectory)
{
	m_shape = std::make_shared<Shape>();

	auto mtlDir = resourceDirectory + "Scene1/Astronaut/";
	m_shape->loadMesh(resourceDirectory + "Scene1/Astronaut/Astronaut.obj", &mtlDir, stbi_load);

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
    glUniform1f(m_shaderProgram->getUniform("ao"), 0.02f);
	m_shape->draw(m_shaderProgram);			//render!!!!!!!

	m_shaderProgram->unbind();
}

void Astronaut::addKeyframes()
{
	addPositionKeyframes(0, {
        Keyframe({499.0000, 3.3685}, {335.0000, 3.3685}, {663.0000, 3.3685}),
        Keyframe({991.0000, 3.6449}, {827.0000, 3.6449}, {1155.0000, 3.6449}),
	});

	addPositionKeyframes(1, {
        Keyframe({499.0000, -3.3387}, {335.0000, -3.3387}, {663.0000, -3.3387}),
        Keyframe({991.0000, -3.6877}, {827.0000, -3.6877}, {1155.0000, -3.6877}),
	});

	addPositionKeyframes(2, {
        Keyframe({499.0000, 2.7902}, {335.0000, 2.7902}, {663.0000, 2.7902}),
        Keyframe({991.0000, 3.7183}, {827.0000, 3.7183}, {1155.0000, 3.7183}),
	});

	addRotationKeyframes(0, {
        Keyframe({499.0000, 0.7994}, {335.0000, 0.7994}, {663.0000, 0.7994}),
        Keyframe({991.0000, 0.6898}, {827.0000, 0.6898}, {1155.0000, 0.6898}),
	});

	addRotationKeyframes(1, {
        Keyframe({499.0000, -0.1031}, {335.0000, -0.1031}, {663.0000, -0.1031}),
        Keyframe({991.0000, 0.3196}, {827.0000, 0.3196}, {1155.0000, 0.3196}),
	});

	addRotationKeyframes(2, {
        Keyframe({499.0000, 0.3835}, {335.0000, 0.3835}, {663.0000, 0.3835}),
        Keyframe({991.0000, 0.1858}, {827.0000, 0.1858}, {1155.0000, 0.1858}),
	});

	addRotationKeyframes(3,{
        Keyframe({499.0000, -0.4509}, {335.0000, -0.4509}, {663.0000, -0.4509}),
        Keyframe({991.0000, -0.6226}, {827.0000, -0.6226}, {1155.0000, -0.6226}),
	});
}
