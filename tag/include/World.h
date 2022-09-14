#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <vector>

#include "Player.h"
#include "Rect.h"

class World
{
public:
    World(glm::vec2 size, int numPlayers);

    glm::vec2 keepInBounds(const glm::vec2& objPos, const glm::vec2& objExtents) const;

    float getAspectRatio() const;

    const glm::vec2& getSize() const
    {
        return size;
    }

    const glm::vec2& getExtents() const
    {
        return extents;
    }

    std::vector<Player>& getPlayers()
    {
        return players;
    }

    Player* getTaggedPlayer()
    {
        return taggedPlayer;
    }

    void setTaggedPlayer(Player* player)
    {
        taggedPlayer = player;
    }

    void reset();
    void reset(int numPlayers);

public:
    static constexpr int minPlayers = 2;
    static constexpr int maxPlayers = 4;

private:
    /**
     * Absolute (positive) position of a player's spawn location.
     */
    static constexpr glm::vec2 playerOrigin = { 4.f, 3.f };

    glm::vec2 size;
    glm::vec2 extents;
    std::vector<Player> players;
    Player* taggedPlayer = nullptr;
};
