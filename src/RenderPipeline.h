#pragma once

#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Program.h"

// TODO: Integrate this with the rest of the program.
// Start by loading the shaders properly in the accompanying source file.

class RenderPipeline
{
public:
    RenderPipeline(const std::string& resourceDirectory);
    ~RenderPipeline();

    void preRender(GLFWwindow* window);
    void postRender();

private:
    Program* m_pPostShader;
    Program* m_pBlurShader;
    Program* m_pBloomShader;

    GLuint m_primaryFrameBuffer;
    GLuint m_primaryColorBuffer;
    //GLuint m_fxaaFrameBuffer;
    //GLuint m_fxaaColorBuffer;
    GLuint m_hdrFrameBuffer;
    GLuint m_hdrColorBuffer;
    GLuint m_pingPongFrameBuffers[2];
    GLuint m_pingPongColorBuffers[2];
    GLuint m_depthRenderBuffer;
    GLuint m_quadVertexArrayObject;
    GLuint m_quadVertexBufferObject;
    GLuint m_quadTextureBufferObject;

    int m_bufferWidth;
    int m_bufferHeight;
    bool m_areFrameBuffersDirty;
    bool m_hasFrameBuffers;

    void loadShaders(const std::string& resourceDirectory);
    void createFrameBuffers();
    inline void deleteBuffers();
};
