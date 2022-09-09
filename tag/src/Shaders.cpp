#include "Shaders.h"

#include <iostream>
#include <stdexcept>

namespace Shaders {

///////////////////////////////////////////////////////////////////////////
// Utility methods
///////////////////////////////////////////////////////////////////////////

void printProgramLog(const GLuint program)
{
    if (!glIsProgram(program))
    {
        printf("Name %d is not a program\n", program);
        return;
    }

    // Program log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    // Get info string length
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // Allocate string
    char* infoLog = new char[maxLength];

    // Get info log
    glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
        // Print Log
        printf("%s\n", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
}

void printShaderLog(const GLuint shader)
{
    if (!glIsShader(shader))
    {
        printf("Name %d is not a shader\n", shader);
    }

    // Shader log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    // Get info string length
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    // Allocate string
    char* infoLog = new char[maxLength];

    // Get info log
    glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
        // Print Log
        printf("%s\n", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
}

///////////////////////////////////////////////////////////////////////////
// Shader
///////////////////////////////////////////////////////////////////////////

bool Shader::validateVertexAttribute(GLint attributeLoc, std::string attributeName) const
{
    if (attributeLoc == -1)
    {
        std::cout << "Could not locate vertex attribute " << attributeName << " for shader " << getName() << "\n";
        return false;
    }
    return true;
}

bool Shader::validateUniform(GLint uniformLoc, std::string uniformName) const
{
    if (uniformLoc == -1)
    {
        std::cout << "Could not locate uniform " << uniformLoc << " for shader " << getName() << "\n";
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
// BoxShader
///////////////////////////////////////////////////////////////////////////

BoxShader boxShader;

const char* boxVertShaderSource = R"END_SHADER(
#version 330 core

uniform mat4 view_proj_matrix;

layout(location = 0) in vec2 in_vertex;
layout(location = 1) in vec4 in_color;

out vec4 color;

void main() {
    gl_Position = view_proj_matrix * vec4(in_vertex.x, in_vertex.y, 0.f, 1.f);
    color = in_color;
}
)END_SHADER";

const char* boxFragShaderSource = R"END_SHADER(
#version 330 core

in vec4 color;

out vec4 frag_color;

void main() {
    frag_color = color;
}
)END_SHADER";

void BoxShader::init()
{
    GLuint programId = createShader(boxVertShaderSource, boxFragShaderSource);

    boxShader = BoxShader();
    boxShader.programId = programId;
    boxShader.vertexAttribLoc = glGetAttribLocation(programId, "in_vertex");
    boxShader.colorAttribLoc = glGetAttribLocation(programId, "in_color");
    boxShader.viewProjMatrixUniformLoc = glGetUniformLocation(programId, "view_proj_matrix");

    if (!boxShader.isValid())
    {
        throw std::runtime_error("Failed to create BoxShader");
    }
}

bool BoxShader::isValid() const
{
    // Validate program ID
    if (programId == 0)
    {
        printf("Could not generate program ID\n");
        return false;
    }

    // Validate vertex attributes / uniforms
    return validateVertexAttribute(vertexAttribLoc, "in_vertex")     //
            && validateVertexAttribute(vertexAttribLoc, "in_color")  //
            && validateUniform(viewProjMatrixUniformLoc, "view_proj_matrix");
}

///////////////////////////////////////////////////////////////////////////
// Generic methods
///////////////////////////////////////////////////////////////////////////

void initializeShaders()
{
    BoxShader::init();
}

GLuint createShader(const char* vertShaderSource, const char* fragShaderSource)
{
    GLuint programId = glCreateProgram();

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Read / set vertex source
    glShaderSource(vertexShader, 1, &vertShaderSource, nullptr);

    // Compile vertex source
    glCompileShader(vertexShader);

    // Check vertex shader for errors
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE)
    {
        printf("Unable to compile vertex shader %d!\n", vertexShader);
        printShaderLog(vertexShader);
        return 0;
    }

    // Attach vertex shader to program
    glAttachShader(programId, vertexShader);

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read / set fragment source
    glShaderSource(fragmentShader, 1, &fragShaderSource, nullptr);

    // Compile fragment source
    glCompileShader(fragmentShader);

    // Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
    if (fShaderCompiled != GL_TRUE)
    {
        printf("Unable to compile fragment shader %d!\n", fragmentShader);
        printShaderLog(fragmentShader);
        return 0;
    }

    // Attach fragment shader to program
    glAttachShader(programId, fragmentShader);

    // Link program
    glLinkProgram(programId);

    // Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE)
    {
        printf("Error linking program %d!\n", programId);
        printProgramLog(programId);
        return 0;
    }

    // Clean up
    glDetachShader(programId, vertexShader);
    glDetachShader(programId, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return programId;
}

}  // namespace Shaders
