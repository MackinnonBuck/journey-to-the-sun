#include "Sun.h"

#include <iostream>

void Sun::init(const std::string& resourceDirectory)
{
	m_shaderProgram = std::make_shared<Program>();
	m_shaderProgram->setVerbose(true);
	m_shaderProgram->setShaderNames(resourceDirectory + "/saturn_vertex.glsl", resourceDirectory + "/saturn_fragment.glsl");
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

	auto texLocation = glGetUniformLocation(m_shaderProgram->pid, "tex");//tex, tex2... sampler in the fragment shader
	// Then bind the uniform samplers to texture units:
	glUseProgram(m_shaderProgram->pid);
	glUniform1i(texLocation, 0);
	glUseProgram(0);

    //m_texture = SmartTexture::createTexture(resourceDirectory + "Scene3/Saturn/saturn.png");

	std::string str = resourceDirectory + "/Scene3/Saturn/saturn.png";
	int width, height, channels;
	unsigned char* data = gettextureData(const_cast<char*>(str.c_str()), width, height, channels);
	glGenTextures(1, &m_textureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Sun::render(const glm::mat4& pMatrix, const glm::mat4& vMatrix, float deltaTime)
{
	glm::mat4 mMatrix = glm::rotate(glm::mat4(1.0f), glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		//glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 1000.0f, 0.0f)) *
		//glm::scale(glm::mat4(1.0f), glm::vec3(75.0f));

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	//m_texture->bind(0, 0);

	m_shaderProgram->bind();
	glUniformMatrix4fv(m_shaderProgram->getUniform("P"), 1, GL_FALSE, &pMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("V"), 1, GL_FALSE, &vMatrix[0][0]);
	glUniformMatrix4fv(m_shaderProgram->getUniform("M"), 1, GL_FALSE, &mMatrix[0][0]);

	m_shape->draw(m_shaderProgram);
	m_shaderProgram->unbind();
}
