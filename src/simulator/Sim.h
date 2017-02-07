#include "../physics/physics.h"
#include "../logger/Logger.h"
#include "../exceptionhandler/ExceptionHandle.h"

static Body *BodyArray;
static int MaxBodies;
static int CurrElement;

int initSimulator(int);

void addBody(Body);

Body getBody(int);

Body getCurrentBody();

Body* getBodyArrPtr();

int getBodyArrSize();

void updateSimulator(double dt, int, double);

int resetSim();

Trajectory* getTrajectory(float, float, Line, Color, double, int);
