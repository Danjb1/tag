#pragma once

#include <glm/vec2.hpp>

struct Rect
{
    glm::vec2 pos {};
    glm::vec2 extents {};

    Rect();
    Rect(glm::vec2 pos, glm::vec2 extents);

    bool intersects(Rect other) const;
};
