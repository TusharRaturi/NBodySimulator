#include "Sim.h"

int initSimulator(int MaxBodyCount)
{
    int i = 0;

    BodyArray = (Body*)malloc(MaxBodyCount * sizeof(Body));

    if(!BodyArray)
        return 0;

    CurrElement = -1;

    for(i=0;i<MaxBodyCount;i++)
        BodyArray[i] = getNullBody();

    MaxBodies = MaxBodyCount;

    return 1;
}

void addBody(Body b)
{
    Log("Added body.");
    if(CurrElement >= MaxBodies-1)
        return;

    BodyArray[++CurrElement] = b;
}

Body getBody(int index)
{
    if(index <= CurrElement && CurrElement != -1)
        return BodyArray[index];
    else
        return getNullBody();
}

Body getCurrentBody()
{
    if(CurrElement != -1)
        return BodyArray[CurrElement];
    else
        return getNullBody();
}

Body* getBodyArrPtr()
{
    return BodyArray;
}

int getBodyArrSize()
{
    return CurrElement+1;
}

void updateSimulator(double dt, int collisionType, double distance)
{
    //We can also do simple integration or Euler method integration. Euler is the simplification of the RK4
    //We have to send the whole array because it would not work other wise
    integrateSimple(BodyArray, CurrElement+1, dt, collisionType, distance);
}

int resetSim()
{
    int i = 0;

    CurrElement = -1;

    for(;i<MaxBodies;i++)
        BodyArray[i] = getNullBody();
    Log("Resetting.");
}

Trajectory* getTrajectory(float mass, float radius, Line dragLine, Color cl, double distance, int collision)
{
    Body c;
    int trajSize = MAX_POINTS;
    int g=0;
    Body *ShadowArray = (Body*)malloc((CurrElement+2)*sizeof(Body));
    Point coords;
    Vec2d initVelocity;
    Trajectory traj;

    if(CurrElement == -1)
    {
        traj.points = NULL;
        return &traj;
    }

    traj.points = GlobalAlloc(GPTR, MAX_POINTS * sizeof(Point));

    for(g = 0; g < trajSize; g++)
    {
        traj.points[g].x = 0.0f;
        traj.points[g].y = 0.0f;
    }
    traj.clrr = cl;

    for(g=0;g<=CurrElement;g++)
    {
        ShadowArray[g] = getCopyBody(BodyArray[g]);
    }

    coords = getNewPoint(dragLine.b.x, dragLine.b.y);
    initVelocity = getNewVec2d((dragLine.a.x - dragLine.b.x)/(100.0), (dragLine.a.y - dragLine.b.y)/(100.0));


    ShadowArray[CurrElement+1] = getNewBody(coords, initVelocity, mass, radius, cl);

    int i = 0, j = 0, k = 0;

    traj.size = trajSize;
    traj.points[0] = coords;

    for(i=1;i<trajSize;i++)
    {
        integrateEuler(ShadowArray, CurrElement+2, 1.0, 0, collision, 1, distance);
        traj.points[i].x = ShadowArray[CurrElement+1].coordinates.x;
        traj.points[i].y = ShadowArray[CurrElement+1].coordinates.y;
    }

    return &traj;
}
