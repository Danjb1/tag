#include "Application.h"

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <random>
#include <vector>

#include "Rect.h"

Application::Application(GLFWwindow* window)
    : window(window)
    , world(worldSize, defaultNumPlayers)
    , renderer(window, &world)
{
}

void Application::tick()
{
    if (!playing)
    {
        return;
    }

    // Move all players
    std::vector<Player>& players = world.getPlayers();
    for (Player& player : players)
    {
        player.tick();

        if (player.hasWon())
        {
            playing = false;
            return;
        }
    }

    // Check for collisions between all players
    for (int i = 0; i < players.size(); ++i)
    {
        Player& player = players[i];

        for (int j = i + 1; j < players.size(); ++j)
        {
            Player& other = players[j];

            if (player.getRect().intersects(other.getRect()))
            {
                player.setIntersecting(other);
                other.setIntersecting(player);

                // Only tag once per tick, and only if players were not already intersecting
                if (!player.wasIntersecting(other))
                {
                    tag(player, other);
                }
            }
        }
    }

    for (Player& player : players)
    {
        player.endTick();
    }
}

void Application::render()
{
    renderer.render();
}

bool Application::isRunning() const
{
    return !glfwWindowShouldClose(window);
}

void Application::keyPressed(int key, int mods)
{
    // Fullscreen toggle
    if (key == GLFW_KEY_ENTER && mods | GLFW_MOD_ALT)
    {
        toggleFullscreen();
        return;
    }

    // Fullscreen emergency exit
    if (key == GLFW_KEY_ESCAPE && windowProps.fullscreen)
    {
        toggleFullscreen();
        return;
    }

    // Restart
    if (key == GLFW_KEY_SPACE)
    {
        restart();
        return;
    }

    // Add / remove players
    if (key == GLFW_KEY_F2)
    {
        world.reset(2);
        return;
    }
    if (key == GLFW_KEY_F3)
    {
        world.reset(3);
        return;
    }
    if (key == GLFW_KEY_F4)
    {
        world.reset(4);
        return;
    }

    // Player 1
    if (key == GLFW_KEY_UP)
    {
        world.getPlayers()[0].setDir(Direction::UP);
        return;
    }
    if (key == GLFW_KEY_DOWN)
    {
        world.getPlayers()[0].setDir(Direction::DOWN);
        return;
    }
    if (key == GLFW_KEY_LEFT)
    {
        world.getPlayers()[0].setDir(Direction::LEFT);
        return;
    }
    if (key == GLFW_KEY_RIGHT)
    {
        world.getPlayers()[0].setDir(Direction::RIGHT);
        return;
    }

    // Player 2
    if (key == GLFW_KEY_W)
    {
        world.getPlayers()[1].setDir(Direction::UP);
        return;
    }
    if (key == GLFW_KEY_S)
    {
        world.getPlayers()[1].setDir(Direction::DOWN);
        return;
    }
    if (key == GLFW_KEY_A)
    {
        world.getPlayers()[1].setDir(Direction::LEFT);
        return;
    }
    if (key == GLFW_KEY_D)
    {
        world.getPlayers()[1].setDir(Direction::RIGHT);
        return;
    }

    // Player 3
    if (key == GLFW_KEY_KP_8)
    {
        world.getPlayers()[2].setDir(Direction::UP);
        return;
    }
    if (key == GLFW_KEY_KP_5)
    {
        world.getPlayers()[2].setDir(Direction::DOWN);
        return;
    }
    if (key == GLFW_KEY_KP_4)
    {
        world.getPlayers()[2].setDir(Direction::LEFT);
        return;
    }
    if (key == GLFW_KEY_KP_6)
    {
        world.getPlayers()[2].setDir(Direction::RIGHT);
        return;
    }

    // Player 4
    if (key == GLFW_KEY_I)
    {
        world.getPlayers()[3].setDir(Direction::UP);
        return;
    }
    if (key == GLFW_KEY_K)
    {
        world.getPlayers()[3].setDir(Direction::DOWN);
        return;
    }
    if (key == GLFW_KEY_J)
    {
        world.getPlayers()[3].setDir(Direction::LEFT);
        return;
    }
    if (key == GLFW_KEY_L)
    {
        world.getPlayers()[3].setDir(Direction::RIGHT);
        return;
    }
}

void Application::windowResized()
{
    renderer.updateViewport(window);
}

void Application::toggleFullscreen()
{
    bool wasFullscreen = windowProps.fullscreen;
    windowProps.fullscreen = !windowProps.fullscreen;

    if (wasFullscreen)
    {
        // Exit fullscreen - restore previous window settings
        glfwSetWindowMonitor(window, nullptr, windowProps.x, windowProps.y, windowProps.width, windowProps.height, 0);
    }
    else
    {
        // Enter fullscreen - remember current window settings
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
        glfwGetWindowSize(window, &windowProps.width, &windowProps.height);
        glfwGetWindowPos(window, &windowProps.x, &windowProps.y);
        glfwSetWindowMonitor(window, monitor, 0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);
    }
}

void Application::restart()
{
    world.reset();

    playing = true;
}

void Application::tag(Player& a, Player& b)
{
    Player* taggedPlayer = world.getTaggedPlayer();

    if (!taggedPlayer)
    {
        // Pick a player at random
        std::random_device seedGen;
        std::mt19937 rng(seedGen());
        std::uniform_real_distribution<float> dist(0.f, 1.f);
        taggedPlayer = dist(rng) < 0.5f ? &a : &b;
        world.setTaggedPlayer(taggedPlayer);
        return;
    }

    taggedPlayer->resetSpeed();

    if (taggedPlayer == &a)
    {
        world.setTaggedPlayer(&b);
    }
    else if (taggedPlayer == &b)
    {
        world.setTaggedPlayer(&a);
    }
}
