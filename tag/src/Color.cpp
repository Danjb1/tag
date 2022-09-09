#include "Color.h"

const Color Color::white = { 1.f, 1.f, 1.f, 1.f };
const Color Color::red = { 1.f, 0.f, 0.f, 1.f };
const Color Color::green = { 0.f, 1.f, 0.f, 1.f };

Color::Color(float r, float g, float b, float a)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
{
}
