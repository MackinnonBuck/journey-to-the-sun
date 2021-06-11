#include "RenderPipeline.h"

#include <iostream>

RenderPipeline::RenderPipeline(const std::string& resourceDirectory) :
    m_areFrameBuffersDirty(true),
    m_hasFrameBuffers(false),
    m_bufferWidth(-1),
    m_bufferHeight(-1),
    m_depthRenderBuffer(0),
    m_primaryFrameBuffer(0),
    m_primaryColorBuffer(0),
    //m_fxaaFrameBuffer(0),
    //m_fxaaColorBuffer(0),
    m_hdrFrameBuffer(0),
    m_hdrColorBuffer(0),
    m_pingPongFrameBuffers{0, 0},
    m_pingPongColorBuffers{0, 0}
{
    loadShaders(resourceDirectory);

    m_pPostShader->bind();
    glUniform1i(m_pPostShader->getUniform("screenTexture"), 0);
    m_pPostShader->unbind();

    m_pBlurShader->bind();
    glUniform1i(m_pBlurShader->getUniform("screenTexture"), 0);
    m_pBlurShader->unbind();

    m_pBloomShader->bind();
    glUniform1i(m_pBloomShader->getUniform("screenTexture"), 0);
    glUniform1i(m_pBloomShader->getUniform("blurTexture"), 1);
    glUniform1i(m_pBloomShader->getUniform("bloom"), 1);
    glUniform1f(m_pBloomShader->getUniform("exposure"), 1.0f);
    m_pBloomShader->unbind();

    GLfloat quadVertices[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };

    GLfloat quadTextureCoordinates[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    glGenVertexArrays(1, &m_quadVertexArrayObject);
    glBindVertexArray(m_quadVertexArrayObject);

    glGenBuffers(1, &m_quadVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVertexBufferObject);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &m_quadTextureBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadTextureBufferObject);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadTextureCoordinates), quadTextureCoordinates, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
}

RenderPipeline::~RenderPipeline()
{
    if (m_bufferWidth != -1 || m_bufferHeight != -1)
        deleteBuffers();

    glDeleteBuffers(1, &m_quadVertexBufferObject);
    glDeleteBuffers(1, &m_quadTextureBufferObject);
    glDeleteVertexArrays(1, &m_quadVertexArrayObject);
}

void RenderPipeline::preRender(GLFWwindow* window)
{
	int width;
    int height;

    glfwGetFramebufferSize(window, &width, &height);

    if (width != m_bufferWidth || height != m_bufferHeight)
    {
        m_bufferWidth = width;
        m_bufferHeight = height;
        m_areFrameBuffersDirty = true;
    }

    if (m_areFrameBuffersDirty)
    {
        m_areFrameBuffersDirty = false;
        createFrameBuffers();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_primaryFrameBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderPipeline::postRender()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glBindVertexArray(m_quadVertexArrayObject);

    glActiveTexture(GL_TEXTURE0);

    //glBindFramebuffer(GL_FRAMEBUFFER, m_fxaaFrameBuffer);

    //m_pFxaaShader->bind();

    //glUniform1f(m_pFxaaShader->getUniform("screenWidth"), (float)m_bufferWidth);
    //glUniform1f(m_pFxaaShader->getUniform("screenHeight"), (float)m_bufferHeight);

    //glBindTexture(GL_TEXTURE_2D, m_primaryColorBuffer);
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    //m_pFxaaShader->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFrameBuffer);

    m_pPostShader->bind();

    //glBindTexture(GL_TEXTURE_2D, m_fxaaColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_primaryColorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_pPostShader->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    bool horizontal = true, firstIteration = true;
    int amount = 200;//30;

    m_pBlurShader->bind();

    for (int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingPongFrameBuffers[horizontal]);
        glUniform1i(m_pBlurShader->getUniform("horizontal"), horizontal);

        glBindTexture(GL_TEXTURE_2D, firstIteration ? m_hdrColorBuffer : m_pingPongColorBuffers[!horizontal]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        horizontal = !horizontal;
        
        if (firstIteration)
            firstIteration = false;
    }

    m_pBlurShader->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //int x = (int)(m_defaultBufferWidth * m_offsetRatio.x);
    //int y = (int)(m_defaultBufferHeight * m_offsetRatio.y);

    glViewport(0, 0, m_bufferWidth, m_bufferHeight);

    m_pBloomShader->bind();

    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, m_fxaaColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_primaryColorBuffer);
    //glBindTexture(GL_TEXTURE_2D, m_hdrColorBuffer);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_pingPongColorBuffers[!horizontal]);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    m_pBloomShader->unbind();

    glEnable(GL_DEPTH_TEST);
}

void RenderPipeline::loadShaders(const std::string& resourceDirectory)
{
    //AssetManager* pAssets = Game::getInstance().getScene()->getAssetManager();

    //m_pPostShader = pAssets->getShaderProgram("postShader");
    //m_pFxaaShader = pAssets->getShaderProgram("fxaaShader");
    //m_pBlurShader = pAssets->getShaderProgram("blurShader");
    //m_pBloomShader = pAssets->getShaderProgram("bloomShader");

	m_pPostShader = new Program();
	m_pPostShader->setVerbose(true);
	m_pPostShader->setShaderNames(resourceDirectory + "post_vertex.glsl", resourceDirectory + "/post_fragment.glsl");
	if (!m_pPostShader->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
    m_pPostShader->addUniform("screenTexture");
	m_pPostShader->addAttribute("vertexPosition");
	m_pPostShader->addAttribute("vertexTexture");

    // TODO: FXAA?

    m_pBlurShader = new Program();
	m_pBlurShader->setVerbose(true);
	m_pBlurShader->setShaderNames(resourceDirectory + "blur_vertex.glsl", resourceDirectory + "/blur_fragment.glsl");
	if (!m_pBlurShader->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
    m_pBlurShader->addUniform("screenTexture");
    m_pBlurShader->addUniform("horizontal");
	m_pBlurShader->addAttribute("vertexPosition");
	m_pBlurShader->addAttribute("vertexTexture");

    m_pBloomShader = new Program();
	m_pBloomShader->setVerbose(true);
	m_pBloomShader->setShaderNames(resourceDirectory + "bloom_vertex.glsl", resourceDirectory + "/bloom_fragment.glsl");
	if (!m_pBloomShader->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
    m_pBloomShader->addUniform("screenTexture");
    m_pBloomShader->addUniform("blurTexture");
    m_pBloomShader->addUniform("bloom");
    m_pBloomShader->addUniform("exposure");
	m_pBloomShader->addAttribute("vertexPosition");
	m_pBloomShader->addAttribute("vertexTexture");
}

void RenderPipeline::createFrameBuffers()
{
    if (m_hasFrameBuffers)
    {
        deleteBuffers();
    }

    glGenFramebuffers(1, &m_primaryFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_primaryFrameBuffer);

    glGenTextures(1, &m_primaryColorBuffer);

    glBindTexture(GL_TEXTURE_2D, m_primaryColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_bufferWidth, m_bufferHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_primaryColorBuffer, 0);

    glGenRenderbuffers(1, &m_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_bufferWidth, m_bufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GLenum frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Unable to create primary frame buffer: " << frameBufferStatus << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    //glGenFramebuffers(1, &m_fxaaFrameBuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER, m_fxaaFrameBuffer);

    //glGenTextures(1, &m_fxaaColorBuffer);

    //glBindTexture(GL_TEXTURE_2D, m_fxaaColorBuffer);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_bufferWidth, m_bufferHeight, 0, GL_RGB, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fxaaColorBuffer, 0);

    //frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    //if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE)
    //    std::cerr << "Unable to create FXAA frame buffer: " << frameBufferStatus << std::endl;

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &m_hdrFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFrameBuffer);

    glGenTextures(1, &m_hdrColorBuffer);

    glBindTexture(GL_TEXTURE_2D, m_hdrColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_bufferWidth, m_bufferHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hdrColorBuffer, 0);

    frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Unable to create HDR frame buffer: " << frameBufferStatus << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(2, m_pingPongFrameBuffers);
    glGenTextures(2, m_pingPongColorBuffers);

    for (int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingPongFrameBuffers[i]);
        glBindTexture(GL_TEXTURE_2D, m_pingPongColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_bufferWidth, m_bufferHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingPongColorBuffers[i], 0);

        GLenum frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Unable to create ping pong frame buffer " << i << ": " << frameBufferStatus << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    m_hasFrameBuffers = true;
}

void RenderPipeline::deleteBuffers()
{
    glDeleteFramebuffers(1, &m_primaryFrameBuffer);
    //glDeleteFramebuffers(1, &m_fxaaFrameBuffer);
    glDeleteFramebuffers(1, &m_hdrFrameBuffer);
    glDeleteFramebuffers(2, m_pingPongFrameBuffers);
    glDeleteRenderbuffers(1, &m_depthRenderBuffer);
    glDeleteTextures(1, &m_primaryColorBuffer);
    //glDeleteTextures(1, &m_fxaaColorBuffer);
    glDeleteTextures(1, &m_hdrColorBuffer);
    glDeleteTextures(2, m_pingPongColorBuffers);
}
