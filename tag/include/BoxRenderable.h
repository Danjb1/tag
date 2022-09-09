#pragma once

#include <gl/glew.h>

#include <vector>

struct Color;
struct Rect;

class BoxRenderScope;

/**
 * Class that allows one or more coloured boxes to be rendered.
 *
 * This creates the VAO and all necessary buffers.
 */
class BoxRenderable
{
    friend class BoxRenderScope;

public:
    static constexpr int numVertexDimensions = 2;  // x, y
    static constexpr int numColorDimensions = 4;   // r, g, b, a
    static constexpr int numVerticesPerBox = 4;

    /**
     * Constructs a BoxRenderable.
     *
     * @param maxBoxes The maximum number of boxes that can be drawn.
     */
    BoxRenderable(int maxBoxes);

    ~BoxRenderable();

    // Disable moving / copying
    BoxRenderable(const BoxRenderable& other) = delete;
    BoxRenderable(BoxRenderable&& other) = delete;
    BoxRenderable& operator=(const BoxRenderable& other) = delete;
    BoxRenderable& operator=(BoxRenderable&& other) = delete;

    BoxRenderScope bind();
    void reset();
    void addBox(const Rect& rect, const Color& color);

private:
    /*
     * 4 indices are required to render a quad using GL_TRIANGLE_FAN:
     *     0------1
     *     | \    |
     *     |   \  |
     *     3----- 2
     */
    static constexpr int numIndicesForTriangleFan = 4;

    /*
     * 6 indices are required to render a quad using GL_TRIANGLES:
     *  - First triangle: 0-1-2
     *  - Second triangle: 2-3-0
     */
    static constexpr int numIndicesForTriangles = 6;

    GLuint vao;
    GLuint positionVbo;
    GLuint colorVbo;
    GLuint ibo;

    std::vector<GLfloat> vertexData;
    std::vector<GLfloat> colorData;

    GLenum drawMode;

    int indicesPerBox;

    int maxBoxes;
    int numBoxes = 0;
};

/**
 * Class that can render a BoxRenderable safely.
 */
class BoxRenderScope
{
    friend class BoxRenderable;

public:
    ~BoxRenderScope();

    operator bool();

    void update() const;
    void render() const;

private:
    BoxRenderScope(BoxRenderable* boxRenderable);

private:
    BoxRenderable* boxRenderable;
};
