#pragma once

#include <glm/vec2.hpp>

#include <unordered_set>

#include "Color.h"
#include "Rect.h"

class World;

enum class Direction : char
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Player
{
public:
    Player(int playerId, World* world, glm::vec2 pos, Color col);

    void tick();
    void endTick();

    void setDir(Direction newDir);
    void setPos(const glm::vec2& newPos);
    void resetSpeed();

    bool wasIntersecting(const Player& other) const;
    void setIntersecting(const Player& other);

    float getTimeRemainingRatio() const;
    bool hasWon() const;

    const Rect getRect() const
    {
        return rect;
    }

    const Color getColor() const
    {
        return col;
    }

    int getPlayerId() const
    {
        return playerId;
    }

private:
    static constexpr glm::vec2 extents { 0.5f, 0.5f };
    static constexpr float baseSpeed = 10.f;
    static constexpr float maxSpeed = 12.5f;
    static constexpr float timeTilMaxSpeed = 7.f;
    static constexpr float acceleration = (maxSpeed - baseSpeed) / timeTilMaxSpeed;
    static constexpr float maxTime = 40.f;

    int playerId;
    World* world = nullptr;
    Rect rect;
    Color col;
    Direction dir = Direction::NONE;
    glm::vec2 dirVector { 0.f, 0.f };
    float speed = baseSpeed;
    float timeRemaining = maxTime;

    std::unordered_set<int> intersectingPlayers;
    std::unordered_set<int> intersectingPlayersLastFrame;
};
