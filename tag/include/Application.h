#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include "GameRenderer.h"
#include "Player.h"
#include "World.h"

struct GLFWwindow;

class Application
{
public:
    Application(GLFWwindow* window);

    bool isRunning() const;
    void tick();
    void render();
    void keyPressed(int key);
    void windowResized();

private:
    void restart();
    void tag(Player& a, Player& b);

private:
    static constexpr glm::vec2 worldSize { 24.f, 18.f };
    static constexpr float borderThickness = 0.15f;

    GLFWwindow* window;

    World world;
    GameRenderer renderer;
    bool playing = true;
};
