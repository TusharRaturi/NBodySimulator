#include "NBSGL.h"

int initOGL(RECT rec)
{
    char tmp[100];

    sprintf(tmp, "%s%s", "OpenGL Version: ", glGetString(GL_VERSION));
    Log(tmp);

    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glMatrixMode(GL_PROJECTION);
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	ResizeCanvas(rec);

	free(tmp);

	return 1;
}

void ResizeCanvas(RECT rec)
{
    if (rec.bottom==0)										// Prevent A Divide By Zero By
	{
		rec.bottom=1;										// Making Height Equal One
	}

	glViewport(0,0,rec.right, rec.bottom);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	glOrtho(0.0f, rec.right, rec.bottom, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void DrawOGL(HDC hDC)
{
    SwapBuffers(hDC);
}

void drawCircularBody(Body b, float accuracy)
{
    if(isBodyNull(b))
        return;

    float i = 0;

    glColor4f(b.color.r, b.color.g, b.color.b, b.color.a);

    glBegin(GL_POLYGON);
    for(i=0; i < (2 * M_PI); i += accuracy)
    {
        glVertex2f(b.r * cos(i) + b.coordinates.x, b.r * sin(i) + b.coordinates.y);
    }
    glEnd();
}

// Here's Where We Do All The Drawing
int DrawGLScene(Body *BodyArray, Line line, int size, Trajectory *trajectory, float accuracy, int TrajCheck, double px, double py)
{
    int i = 0;

    glClearColor(0.1f, 0.1f, 0.1f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glTranslatef(px, py, 0);

    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
        glVertex2f(line.a.x, line.a.y);
        glVertex2f(line.b.x, line.b.y);
    glEnd();

    for(i = 0; i < size; i++)
    {
        drawCircularBody(BodyArray[i], accuracy);
    }

    if(TrajCheck && trajectory != NULL && trajectory->points != NULL)
    {
        glColor4f(trajectory->clrr.r, trajectory->clrr.g, trajectory->clrr.b, trajectory->clrr.a);
        glBegin(GL_POINTS);
        for(i = 0; i < trajectory->size; i++)
        {
            glVertex2f(trajectory->points[i].x, trajectory->points[i].y);
        }
        glEnd();
    }

    glFlush();

    glPopMatrix();
}
