#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "BoxRenderable.h"
#include "Rect.h"
#include "World.h"

struct GLFWwindow;

class GameRenderer
{
public:
    GameRenderer(GLFWwindow* window, World* world, float borderThickness);
    void updateViewport(GLFWwindow* window);
    void render();

private:
    /**
     * Creates a view-projection matrix to frame the given Border.
     */
    glm::mat4 makeViewProjectionMatrix(const World& world);

    /**
     * Sets the viewport to fill the window, preserving the given aspect ratio.
     */
    void fillWindow(GLFWwindow* window, float aspectRatio);

    /**
     * Creates a Rect to represent a player's score.
     */
    Rect makeScoreRect(const Player& player) const;

private:
    /**
     * Near plane used for our camera projection.
     */
    static constexpr float nearPlane = -1.f;

    /**
     * Far plane used for our camera projection.
     */
    static constexpr float farPlane = 1.f;

    /**
     * World up vector.
     */
    static constexpr glm::vec3 up = glm::vec3(0.f, -1.f, 0.f);

    /**
     * Offset applied to the camera position to fit the UI in frame.
     */
    static constexpr glm::vec3 cameraOffset = 0.75f * up;

    /**
     * Point in the world where the camera should look at.
     */
    static constexpr glm::vec3 cameraLookAt = glm::vec3(0.f, 0.f, 0.f) + cameraOffset;

    /**
     * Position of the camera in the world.
     */
    static constexpr glm::vec3 cameraPos = glm::vec3(0.f, 0.f, -1.f) + cameraOffset;

    /**
     * Padding to add around the game border during rendering.
     *
     * This should match the aspect ratio of the world, to prevent stretching.
     */
    static constexpr glm::vec2 borderPadding { 4.f, 3.f };

    /**
     * Offset from the top of the game area at which to render scores.
     */
    static constexpr glm::vec2 scoreOffset = 2.f * up;

    /**
     * Height of the score bars.
     */
    static constexpr float scoreHeight = 1.f;

    /**
     * Padding to add between player scores.
     */
    static constexpr float scorePadding = 1.f;

    static constexpr int numBoxesForBorder = 4;
    static constexpr int numBoxesForPlayers = 2 * World::maxPlayers;
    static constexpr int maxBoxesToRender = numBoxesForBorder + numBoxesForPlayers;

    Rect borderTop;
    Rect borderLeft;
    Rect borderBottom;
    Rect borderRight;

    World* world;
    BoxRenderable boxRenderable { maxBoxesToRender };
};
