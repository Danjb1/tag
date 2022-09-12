#include "World.h"

#include "MathUtils.h"

World::World(glm::vec2 size, int numPlayers)
    : size(size)
    , extents(size.x / 2.f, size.y / 2.f)
{
    reset(numPlayers);
}

float World::getAspectRatio() const
{
    return size.x / size.y;
}

glm::vec2 World::keepInBounds(const glm::vec2& objPos, const glm::vec2& objExtents) const
{
    float x = MathUtils::clamp(objPos.x, -extents.x + objExtents.x, extents.x - objExtents.x);
    float y = MathUtils::clamp(objPos.y, -extents.y + objExtents.y, extents.y - objExtents.y);
    return { x, y };
}

void World::reset()
{
    reset(static_cast<int>(players.size()));
}

void World::reset(int numPlayers)
{
    taggedPlayer = nullptr;

    players.clear();

    // Player 1
    float p1x = numPlayers == 3 ? 0.f : -playerOrigin.x;
    players.emplace_back(0, this, glm::vec2(p1x, -playerOrigin.y), Color::red);

    // Player 2
    players.emplace_back(1, this, glm::vec2(playerOrigin.x, playerOrigin.y), Color::green);

    // Player 3
    if (numPlayers > 2)
    {
        players.emplace_back(2, this, glm::vec2(-playerOrigin.x, playerOrigin.y), Color::blue);
    }

    // Player 4
    if (numPlayers > 3)
    {
        players.emplace_back(3, this, glm::vec2(playerOrigin.x, -playerOrigin.y), Color::yellow);
    }
}
