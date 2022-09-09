#include "BoxRenderable.h"

#include <iostream>  // tmp
#include <stdexcept>

#include "Color.h"
#include "Rect.h"
#include "Shaders.h"

BoxRenderable::BoxRenderable(int maxBoxes)
    : maxBoxes(maxBoxes)
{
    // Generate VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBOs
    glGenBuffers(1, &positionVbo);
    glGenBuffers(1, &colorVbo);
    glGenBuffers(1, &ibo);

    // Determine the primitive that should be drawn
    drawMode = (maxBoxes == 1) ? GL_TRIANGLE_FAN : GL_TRIANGLES;

    // Determine the number of indices per box
    indicesPerBox = (drawMode == GL_TRIANGLE_FAN) ? numIndicesForTriangleFan : numIndicesForTriangles;

    // Allocate space to hold vertex positions
    size_t positionBufferSize = maxBoxes * numVertexDimensions * indicesPerBox * sizeof(GLfloat);
    vertexData.reserve(positionBufferSize);

    // Initialize vertex buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(
            Shaders::vertexAttribIndex,
            numVertexDimensions,
            GL_FLOAT,
            GL_FALSE,
            numVertexDimensions * sizeof(GLfloat),
            nullptr);
    glBufferData(GL_ARRAY_BUFFER, positionBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Allocate space to hold vertex colors
    size_t colorBufferSize = maxBoxes * numColorDimensions * indicesPerBox * sizeof(GLfloat);
    colorData.reserve(colorBufferSize);

    // Initialize color buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glVertexAttribPointer(
            Shaders::colorAttribIndex,
            numColorDimensions,
            GL_FLOAT,
            GL_FALSE,
            numColorDimensions * sizeof(GLfloat),
            nullptr);
    glBufferData(GL_ARRAY_BUFFER, colorBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize index buffer - this should never need to change
    std::vector<GLuint> indexData;
    indexData.reserve(maxBoxes * indicesPerBox);
    for (int i = 0; i < maxBoxes; i++)
    {
        unsigned int startIndex = i * numVerticesPerBox;

        if (drawMode == GL_TRIANGLE_FAN)
        {
            indexData.push_back(startIndex);
            indexData.push_back(startIndex + 1);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 3);
        }
        else if (drawMode == GL_TRIANGLES)
        {
            indexData.push_back(startIndex);
            indexData.push_back(startIndex + 1);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 3);
            indexData.push_back(startIndex);
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxBoxes * indicesPerBox * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

    // Enable vertex attributes
    glEnableVertexAttribArray(Shaders::vertexAttribIndex);
    glEnableVertexAttribArray(Shaders::colorAttribIndex);
}

BoxRenderable::~BoxRenderable()
{
    glDeleteBuffers(1, &positionVbo);
    glDeleteBuffers(1, &colorVbo);
    glDeleteVertexArrays(1, &vao);
}

BoxRenderScope BoxRenderable::bind()
{
    return BoxRenderScope(this);
}

void BoxRenderable::reset()
{
    vertexData.clear();
    colorData.clear();
    numBoxes = 0;
}

void BoxRenderable::addBox(const Rect& rect, const Color& color)
{
    if (numBoxes == maxBoxes)
    {
        throw std::runtime_error("Tried to render too many boxes");
        return;
    }

    // Define vertex positions
    float x1 = rect.pos.x - rect.extents.x;
    float y1 = rect.pos.y - rect.extents.y;
    float x2 = rect.pos.x + rect.extents.x;
    float y2 = rect.pos.y + rect.extents.y;
    std::vector<GLfloat> newVertexData = {
        x1, y1,  //
        x2, y1,  //
        x2, y2,  //
        x1, y2   //
    };

    // Determine vertex colors
    float r = static_cast<float>(color.r);
    float g = static_cast<float>(color.g);
    float b = static_cast<float>(color.b);
    float a = static_cast<float>(color.a);
    std::vector<GLfloat> newColorData = { r, g, b, a,  //
                                          r, g, b, a,  //
                                          r, g, b, a,  //
                                          r, g, b, a };

    // Add the box to our buffers
    vertexData.insert(vertexData.cend(), newVertexData.cbegin(), newVertexData.cend());
    colorData.insert(colorData.cend(), newColorData.cbegin(), newColorData.cend());
    ++numBoxes;
}

BoxRenderScope::BoxRenderScope(BoxRenderable* boxRenderable)
    : boxRenderable(boxRenderable)
{
    glBindVertexArray(boxRenderable->vao);
}

BoxRenderScope::operator bool()
{
    return boxRenderable->vao != 0;
}

BoxRenderScope::~BoxRenderScope()
{
    glBindVertexArray(0);
}

void BoxRenderScope::update() const
{
    // Upload position data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, boxRenderable->positionVbo);
    size_t positionBufferSize = boxRenderable->vertexData.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, boxRenderable->vertexData.data());

    // Upload color data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, boxRenderable->colorVbo);
    size_t colorBufferSize = boxRenderable->colorData.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colorBufferSize, boxRenderable->colorData.data());
}

void BoxRenderScope::render() const
{
    int numVerts = boxRenderable->numBoxes * boxRenderable->indicesPerBox;
    glDrawElements(boxRenderable->drawMode, numVerts, GL_UNSIGNED_INT, nullptr);
}
