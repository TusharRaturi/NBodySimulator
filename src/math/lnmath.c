#include "lnmath.h"

Line getNewLine()
{
    Line tmp;
    tmp.a = getNewPoint(0, 0);
    tmp.b = getNewPoint(0, 0);
    return tmp;
}
