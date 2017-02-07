#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../simulator/Sim.h"

#define WIN_WIDTH 800.0
#define WIN_HEIGHT 600.0

struct GLContext
{
    HDC hDC;                 /* Device context */
    HGLRC hRC;               /* OpenGL Rendering context */
};

typedef struct GLContext GLC;

typedef struct Circle_struct
{
    float x, y;
    float rad;
}Circle;

int initOGL(RECT);

int DrawGLScene(Body *, Line, int, Trajectory*, float, int, double, double);

void DrawOGL(HDC);

void drawCircularBody(Body, float);
