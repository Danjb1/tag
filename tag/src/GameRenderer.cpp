#include "GameRenderer.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Color.h"
#include "Player.h"
#include "Shaders.h"

GameRenderer::GameRenderer(GLFWwindow* window, World* world, float borderThickness)
    : world(world)
{
    // Calculate border Rects
    glm::vec2 worldExtents = world->getExtents();
    glm::vec2 worldExtentsPlusBorder = worldExtents + glm::vec2(borderThickness, borderThickness);
    float halfBorderThickness = borderThickness / 2.f;
    borderTop = { { 0.f, -worldExtents.y - halfBorderThickness }, { worldExtentsPlusBorder.x, halfBorderThickness } };
    borderLeft = { { -worldExtents.x - halfBorderThickness, 0.f }, { halfBorderThickness, worldExtentsPlusBorder.y } };
    borderBottom = { { 0.f, worldExtents.y + halfBorderThickness }, { worldExtentsPlusBorder.x, halfBorderThickness } };
    borderRight = { { worldExtents.x + halfBorderThickness, 0.f }, { halfBorderThickness, worldExtentsPlusBorder.y } };

    // Use shader
    glUseProgram(Shaders::boxShader.programId);

    // Set uniform values
    glm::mat4 viewProjMatrix = makeViewProjectionMatrix(*world);
    glUniformMatrix4fv(Shaders::boxShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);

    updateViewport(window);
}

void GameRenderer::updateViewport(GLFWwindow* window)
{
    fillWindow(window, world->getAspectRatio());
}

void GameRenderer::render()
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Prepare to render
    boxRenderable.reset();

    // - Border
    const Player* taggedPlayer = world->getTaggedPlayer();
    const Color borderColor = taggedPlayer ? taggedPlayer->getColor() : Color::white;
    boxRenderable.addBox(borderTop, borderColor);
    boxRenderable.addBox(borderLeft, borderColor);
    boxRenderable.addBox(borderBottom, borderColor);
    boxRenderable.addBox(borderRight, borderColor);

    // - Players & scores
    for (const Player& player : world->getPlayers())
    {
        boxRenderable.addBox(player.getRect(), player.getColor());
        boxRenderable.addBox(makeScoreRect(player), player.getColor());
    }

    // Render
    if (BoxRenderScope renderScope = boxRenderable.bind())
    {
        renderScope.update();
        renderScope.render();
    }
}

glm::mat4 GameRenderer::makeViewProjectionMatrix(const World& world)
{
    // Determine our view matrix.
    // This assumes our vertices are positioned using this co-ordinate system:
    //  - +x points right
    //  - +y points down
    //  - +z points into the screen
    glm::mat4 view = glm::lookAt(cameraPos, cameraLookAt, up);

    // Determine our projection matrix
    glm::vec2 extents = world.getExtents() + borderPadding;
    glm::mat4 projection = glm::ortho(-extents.x, extents.x, -extents.y, extents.y, nearPlane, farPlane);

    // Combine matrices
    return projection * view;
}

void GameRenderer::fillWindow(GLFWwindow* window, float aspectRatio)
{
    // Get window size
    int outX;
    int outY;
    glfwGetWindowSize(window, &outX, &outY);
    glm::vec2 windowSize(outX, outY);

    // Fill the available width, and set height accordingly
    glm::vec2 viewportSize(windowSize.x, windowSize.x / aspectRatio);

    // Scale down if y is too big to fit
    if (viewportSize.y > windowSize.y)
    {
        viewportSize *= windowSize.y / viewportSize.y;
    }

    // Center vertically
    glm::vec2 viewportOffset = (windowSize - viewportSize) / 2.f;

    glViewport(
            static_cast<GLint>(viewportOffset.x),
            static_cast<GLint>(viewportOffset.y),
            static_cast<GLsizei>(viewportSize.x),
            static_cast<GLsizei>(viewportSize.y));
}

Rect GameRenderer::makeScoreRect(const Player& player) const
{
    // Calculate player ratio
    int index = player.getPlayerId();
    int numPlayers = static_cast<int>(world->getPlayers().size());
    float playerRatio = static_cast<float>(index) / numPlayers;

    // Size
    float widthPerPlayer = (world->getSize().x / numPlayers);
    float maxWidth = widthPerPlayer - scorePadding;
    float width = maxWidth * player.getTimeRemainingRatio();
    glm::vec2 extents = { width / 2.f, scoreHeight / 2.f };

    // Position
    float x = -world->getExtents().x + (playerRatio * world->getSize().x) + (widthPerPlayer / 2.f);
    float y = -world->getExtents().y;
    glm::vec2 pos = glm::vec2(x, y) + scoreOffset;

    return { pos, extents };
}
