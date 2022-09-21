#pragma once

#include <enet.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include "GameRenderer.h"
#include "Player.h"
#include "World.h"

struct GLFWwindow;

struct WindowProperties
{
    int x = -1;
    int y = -1;
    int width = -1;
    int height = -1;
    bool fullscreen = false;
};

class Application
{
public:
    Application(GLFWwindow* window, int numPlayers, ENetHost* connection);

    void run();
    bool isRunning() const;
    void tick();
    void render();
    void keyPressed(int key, int mods);
    void windowResized();
    void toggleFullscreen();

private:
    void restart();
    void tag(Player& a, Player& b);

private:
    static constexpr glm::vec2 worldSize { 24.f, 18.f };

    GLFWwindow* window;
    WindowProperties windowProps;

    ENetHost* connection;

    World world;
    GameRenderer renderer;
    bool playing = true;
};
