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
    , world(worldSize)
    , renderer(window, &world, borderThickness)
{
    restart();
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

void Application::keyPressed(int key)
{
    // Restart
    if (key == GLFW_KEY_SPACE)
    {
        if (!playing)
        {
            restart();
        }
    }

    // Player 1
    else if (key == GLFW_KEY_UP)
    {
        world.getPlayers()[0].setDir(Direction::UP);
    }
    else if (key == GLFW_KEY_DOWN)
    {
        world.getPlayers()[0].setDir(Direction::DOWN);
    }
    else if (key == GLFW_KEY_LEFT)
    {
        world.getPlayers()[0].setDir(Direction::LEFT);
    }
    else if (key == GLFW_KEY_RIGHT)
    {
        world.getPlayers()[0].setDir(Direction::RIGHT);
    }

    // Player 2
    else if (key == GLFW_KEY_W)
    {
        world.getPlayers()[1].setDir(Direction::UP);
    }
    else if (key == GLFW_KEY_S)
    {
        world.getPlayers()[1].setDir(Direction::DOWN);
    }
    else if (key == GLFW_KEY_A)
    {
        world.getPlayers()[1].setDir(Direction::LEFT);
    }
    else if (key == GLFW_KEY_D)
    {
        world.getPlayers()[1].setDir(Direction::RIGHT);
    }
}

void Application::windowResized()
{
    renderer.updateViewport(window);
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
