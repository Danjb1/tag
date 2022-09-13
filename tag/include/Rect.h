#pragma once

#include <glm/vec2.hpp>

struct Rect
{
    glm::vec2 pos {};
    glm::vec2 extents {};

    Rect();
    Rect(glm::vec2 pos, glm::vec2 extents);

    /**
     * Returns a copy of this Rect with an offset applied.
     */
    Rect operator+(const glm::vec2& offset) const;

    bool intersects(Rect other) const;
};
