#include "World.h"

#include "MathUtils.h"

World::World(glm::vec2 size)
    : size(size)
    , extents(size.x / 2.f, size.y / 2.f)
{
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
    taggedPlayer = nullptr;

    players.clear();
    players.emplace_back(0, this, glm::vec2(-playerOrigin.x, -playerOrigin.y), Color::red);
    players.emplace_back(1, this, glm::vec2(playerOrigin.x, playerOrigin.y), Color::green);
}
