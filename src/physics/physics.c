#include "physics.h"

double radiusModifier = 0.000000001483128;
double schwarzschildMultiplier = 7.41564e-28;
double minStarMass = 1e+29;
double chandrashekharLimit = 2e+30;

Body getNullBody()
{
    Body tmp;
    tmp.coordinates.x = tmp.coordinates.y = 0;
    tmp.velocity.x = tmp.velocity.y = 0;
    tmp.m = 0;
    tmp.r = 0;
    tmp.den = 0;
    tmp.color = getNewColor(0, 0, 0, 0);
    tmp.isNull = 1;
    tmp.isStar = 0;
    tmp.isBlackHole = 0;
    return tmp;
}

Body getNewBody(Point cn, Vec2d vn, double mn, double rn, Color clrn)
{
    Body b;
    b.coordinates = cn;
    b.OldCoords = cn;
    b.velocity = vn;
    b.m = mn;
    b.r = rn;
    b.den = mn / (M_PI * rn * rn);
    b.color = clrn;
    b.isNull = 0;
    b.isStar = 0;
    b.isBlackHole = 0;
    return b;
}

int isBodyNull(Body b)
{
    return b.isNull;
}

Body getCopyBody(Body b)
{
    Body tmp;
    tmp.coordinates = b.coordinates;
    tmp.isNull = b.isNull;
    tmp.color = b.color;
    tmp.m = b.m;
    tmp.OldCoords = b.OldCoords;
    tmp.r = b.r;
    tmp.velocity = b.velocity;
    tmp.den = b.den;
    tmp.isStar = b.isStar;
    tmp.isBlackHole = b.isBlackHole;
    return tmp;
}

int CheckCollision(Point coord1, Point coord2, double r1, double r2)
{
    Vec2d v1,v2;

    double d;

    v1 = getNewVec2d(coord1.x, coord1.y);
    v2 = getNewVec2d(coord2.x, coord2.y);

    d = distanceBetween(v1, v2);
    if(r1 + r2 > d)
        return 1;
    return 0;
}

void integrateSimple(Body *bArr, int size, double dt, int collisionType, double distance)
{
    integrateEuler(bArr, size, dt, 1, collisionType, 0, distance);
}

void destroyCollide(Body *a, Body *b, int ps)
{
    a->isNull=b->isNull=1;
    if(ps)
        playSound(addSound("sounds/Exp.mp3"));
}

void blackCollide(Body *a, Body *b)
{
    if(a->m >= b->m)
    {
        afterCollision(b, a);
    }
    else if(a->m < b->m)
    {
        // The schwarzchild radius has a linear relation to mass. This is the approximate:-

        // Blackhole eats mass equal to itself and expands and the body looses mass equal to the blackhole
        b->m -= a->m;
        a->m += a->m;

        // Blackhole gets radius equal to itself and expands and the body looses radius equal to the blackhole
        b->r -= a->r;
        a->r += a->r;

        // Make the body null if it vanishes into nothingness......
        if(b->r<=0)
            b->isNull = 1;

        // Zero their velocities(otherwise they run away from the screen....:P)
        a->velocity.x = a->velocity.y = b->velocity.x = b->velocity.y = 0.0;
    }
}

void mergeCollide(Body *a, Body *b, int destroyEquals)
{
    if(a->isBlackHole && !(b->isBlackHole))
    {
        blackCollide(a, b);
    }
    else if(b->isBlackHole && !(a->isBlackHole))
    {
        blackCollide(b, a);
    }
    else if(a->m == b->m)
    {
        if(destroyEquals)
            destroyCollide(a, b, 1);
        else
        {
            if(a->r < b->r)
                afterCollision(b, a);
            else if(a->r > b->r)
                afterCollision(a, b);
            else
                afterCollision(a, b);
        }
    }

    else if(a->m < b->m)
    {
        afterCollision(b, a);
    }

    else
    {
        afterCollision(a, b);
    }
}

void afterCollision(Body *b, Body *a)
{
    float massPer = (a->m / b->m);
    // Make the new density an average of both
    if(!(a->den > b->den))
        b->den = (a->den * massPer) + (b->den * (1-massPer));

    // Add a's mass to b
    b->m += a->m;

    // Calculate the new radius
    b->r = sqrt(ONE_BY_PI * (b->m / b->den));

    // Conserve momentum
    b->velocity.x = (a->m * a->velocity.x)/b->m;
    b->velocity.y = (a->m * a->velocity.y)/b->m;

    if(b->isBlackHole)
    {
        b->color.r = a->color.g = b->color.b = 0.0;
        *a = getNullBody();
        return;
    }

    if(!(a->color.r > b->color.r))
        b->color.r = (a->color.r * massPer) + (b->color.r * (1-massPer));

    if(!(a->color.g > b->color.g))
        b->color.g = (a->color.g * massPer) + (b->color.g * (1-massPer));

    if(!(a->color.b > b->color.b))
        b->color.b = (a->color.b * massPer) + (b->color.b * (1-massPer));

    *a = getNullBody();
}

void integrateEuler(Body* bArr, int size, double dt, int playSnd, int collisionTechnique, int doTraj, double distance)
{
    int i = 0, j = 0;

    if(!doTraj)
    {
        for(i=0;i<size;i++)
        {
            if(bArr[i].isNull)
                continue;
            else
            {
                if(bArr[i].m >= minStarMass && !bArr[i].isBlackHole)
                {
                    if(!bArr[i].isStar)
                    {
                        bArr[i].isStar = 1;
                        bArr[i].color.r = 1.0;
                        bArr[i].color.g = 0.5;
                        bArr[i].color.b = 0.0;
                    }
                    if(bArr[i].m >= chandrashekharLimit && bArr[i].r * distance * radiusModifier <= schwarzschildMultiplier * bArr[i].m)// Swarzchild rad calculation for blackhole formation
                    {
                        playSound(addSound("sounds/Exp.mp3"));
                        bArr[i].isStar = 0;
                        bArr[i].isBlackHole = 1;
                        bArr[i].color.r = 0.0;
                        bArr[i].color.g = 0.0;
                        bArr[i].color.b = 0.0;
                    }
                }
            }
        }
    }

    for(i=0;i<size;i++)
    {
        Body *a = &bArr[i];
        for(j=0;j<size;j++)
        {
            Body *b = &bArr[j];
            if(i == j)
                continue;
            if(a->isNull == 1 || b->isNull == 1)
                continue;
            if(CheckCollision(a->OldCoords, b->OldCoords, a->r, b->r))
            {
                if(collisionTechnique == COLLISION_DESTROY)
                    destroyCollide(a, b, playSnd);
                else if(collisionTechnique == COLLISION_MERGE_DESTROY)
                    mergeCollide(a, b, 1);
                else if(collisionTechnique == COLLISION_MERGE)
                    mergeCollide(a, b, 0);

            }

            Vec2d sub;
            double r;
            double rSquare;
            sub.x = b->OldCoords.x - a->OldCoords.x;
            sub.y = b->OldCoords.y - a->OldCoords.y;
            r = length(sub) + distance;
            Vec2d rCap = getNewVec2d(sub.x, sub.y);
            rSquare = r * r;
            rCap.x /= r;
            rCap.y /= r;


            a->velocity.x += (6.67408e-11 * b->m * rCap.x) / (rSquare);
            a->velocity.y += (6.67408e-11 * b->m * rCap.y) / (rSquare);

            a->coordinates.x += a->velocity.x;
            a->coordinates.y += a->velocity.y;
        }
    }

    for(i=0;i<size;i++)
    {
        if(!bArr[i].isNull)
        {
            bArr[i].OldCoords = bArr[i].coordinates;
        }
    }
}

void integrateRK4(Body* bArr, int size, double dt)
{
    //TODO(maybe)
}
