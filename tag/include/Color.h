#pragma once

struct Color
{
    float r;
    float g;
    float b;
    float a;

    Color(float r, float g, float b, float a);

    static const Color white;
    static const Color red;
    static const Color green;
    static const Color blue;
    static const Color yellow;
};
