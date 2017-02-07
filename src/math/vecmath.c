#include "vecmath.h"

float length(Vec2d v)
{
    double length;

    length = (v.x * v.x) + (v.y * v.y);

    if(length > 0)
        length = sqrt(length);

    return length;
}

float distanceBetween(Vec2d v1, Vec2d v2)
{
    Vec2d direction;
    subtract(v2, v1, direction);

    return length(direction);
}

Vec2d getNewVec2d(double xn, double yn)
{
    Vec2d t;
    t.x = xn;
    t.y = yn;
    return t;
}
