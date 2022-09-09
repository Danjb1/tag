#include "Player.h"

#include <algorithm>  //max

#include "TimeUtils.h"
#include "World.h"

Player::Player(int playerId, World* world, glm::vec2 pos, Color col)
    : playerId(playerId)
    , world(world)
    , rect(pos, Player::extents)
    , col(col)
{
}

void Player::tick()
{
    setPos(rect.pos + dirVector * speed * TimeUtils::frameTime);

    Player* taggedPlayer = world->getTaggedPlayer();
    if (!taggedPlayer)
    {
        // No-one has been tagged yet
        return;
    }

    if (taggedPlayer == this)
    {
        // Speed up when tagged
        speed = std::min(speed + acceleration * TimeUtils::frameTime, maxSpeed);
    }
    else
    {
        // Count down when not tagged
        timeRemaining = std::max(timeRemaining - TimeUtils::frameTime, 0.f);
    }
}

void Player::endTick()
{
    intersectingPlayersLastFrame = intersectingPlayers;
    intersectingPlayers.clear();
}

void Player::setDir(Direction newDir)
{
    if (newDir == dir)
    {
        // Stop
        dir = Direction::NONE;
        dirVector = { 0.f, 0.f };
        return;
    }

    dir = newDir;

    if (dir == Direction::UP)
    {
        dirVector = { 0.f, -1.f };
    }
    else if (dir == Direction::DOWN)
    {
        dirVector = { 0.f, 1.f };
    }
    else if (dir == Direction::LEFT)
    {
        dirVector = { -1.f, 0.f };
    }
    else if (dir == Direction::RIGHT)
    {
        dirVector = { 1.f, 0.f };
    }
}

void Player::setPos(const glm::vec2& newPos)
{
    rect.pos = world->keepInBounds(newPos, rect.extents);
}

void Player::resetSpeed()
{
    speed = baseSpeed;
}

bool Player::wasIntersecting(const Player& other) const
{
    return intersectingPlayersLastFrame.contains(other.getPlayerId());
}

void Player::setIntersecting(const Player& other)
{
    intersectingPlayers.insert(other.getPlayerId());
}

float Player::getTimeRemainingRatio() const
{
    return timeRemaining / maxTime;
}

bool Player::hasWon() const
{
    return timeRemaining == 0.f;
}
