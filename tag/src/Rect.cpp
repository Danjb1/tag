#include "Rect.h"

Rect::Rect() {}

Rect::Rect(glm::vec2 pos, glm::vec2 extents)
    : pos(pos)
    , extents(extents)
{
}

Rect Rect::operator+(const glm::vec2& offset) const
{
    return { pos + offset, extents };
}

bool Rect::intersects(Rect other) const
{
    // See: https://stackoverflow.com/a/306332/1624459
    glm::vec2 start = pos - extents;
    glm::vec2 end = pos + extents;
    glm::vec2 otherStart = other.pos - other.extents;
    glm::vec2 otherEnd = other.pos + other.extents;
    return start.x < otherEnd.x      //
            && end.x > otherStart.x  //
            && start.y < otherEnd.y  //
            && end.y > otherStart.y;
}
