#include<windows.h>
#include<math.h>
#include "../audio/music.h"
#include "../math/lnmath.h"
#include "../math/vecmath.h"
#include "../opengl/Colour.h"

#define ONE_BY_PI 0.31830988618

#define COLLISION_DESTROY 222
#define COLLISION_MERGE_DESTROY 333
#define COLLISION_MERGE 444

#define UNIT_NANOMETER 0.000000001
#define UNIT_MICROMETER 0.000001
#define UNIT_MILLIMETER 0.001
#define UNIT_CENTIMETER 0.01
#define UNIT_METER 1.0
#define UNIT_KILOMETER 1000.0
#define UNIT_TEST0 2.5e+5
#define UNIT_TEST1 5.0e+6
#define UNIT_TEST2 2.0e+7
#define UNIT_TEST3 7.5e+7
#define UNIT_TEST4 1.5e+8
#define UNIT_LUNAR_DISTANCE 384402000
#define UNIT_ASTRONOMICAL_UNIT 1.496e+11
#define UNIT_LIGHT_YEAR 9.461e+15
#define UNIT_PARSEC 3.086e+16

#define MAX_POINTS 550

typedef struct Elements
{
    int hasIron;
    int hasHydrogen;
    int hasCarbon;
}Elements;

/* The Physical Body Structure */
typedef struct Bodies
{
    Point coordinates;  /* The coordinates of the Body               */
    Point OldCoords;    /* The coordinates of the previous Tick      */
    Vec2d velocity;     /* The velocity of the Body                  */
    double m;            /* The mass of the Body                      */
    double r;            /* The radius of the Body (body is circular) */
    double den;           /* The density of the Body by mass/area */
    Color color;        /* The color of the Body                     */
    //Note: Center of mass will be same as x,y as the body is a circle
    int isNull;
    int isStar;
    int isBlackHole;
    Elements elm;
}Body;

typedef struct Trajectory
{
    Point *points;
    int size;
    Color clrr;
}Trajectory;

Body getNullBody();

Body getNewBody(Point, Vec2d, double, double, Color);

int isBodyNull(Body b);

void integrateSimple(Body*, int, double, int, double);

void integrateEuler(Body*,int, double, int, int, int, double);

void integrateRK4(Body*, int, double);

Body getCopyBody(Body);
