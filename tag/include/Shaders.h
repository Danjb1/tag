#pragma once

#include <gl/glew.h>

#include <string>

namespace Shaders {

// Vertex shader attribute indices.
// We keep these common between all shaders so that our VAOs can be
// rendered by any shader without requiring reconfiguration.
static constexpr GLint vertexAttribIndex = 0;
static constexpr GLint colorAttribIndex = 1;

///////////////////////////////////////////////////////////////////////////
// Shader base class
///////////////////////////////////////////////////////////////////////////

class Shader
{
public:
    virtual std::string getName() const = 0;
    bool validateVertexAttribute(GLint attributeLoc, std::string attributeName) const;
    bool validateUniform(GLint uniformLoc, std::string uniformName) const;
};

///////////////////////////////////////////////////////////////////////////
// TextureShader:
// Renders a texture in 2D space (no depth).
///////////////////////////////////////////////////////////////////////////

class BoxShader : public Shader
{
public:
    GLuint programId;

    // Vertex shader uniform locations
    GLint viewProjMatrixUniformLoc;

    // Vertex shader attribute locations
    GLint vertexAttribLoc;
    GLint colorAttribLoc;

    static void init();

    bool isValid() const;

    std::string getName() const override
    {
        return "BoxShader";
    }
};

extern BoxShader boxShader;

///////////////////////////////////////////////////////////////////////////
// Generic methods
///////////////////////////////////////////////////////////////////////////

void initializeShaders();

GLuint createShader(const char* vertShader, const char* fragShader);

}  // namespace Shaders
