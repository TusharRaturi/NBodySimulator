#include<windowsx.h>
#include "../saveload/SaveLoad.h"
#include "../resources/resource.h"

#define WIN_TITLE "N-Body Gravitational Simulator"

HWND hMainWindow;           /* This is the handle for our main window */
HWND hToolWindow;           /* This is the handle for our toolbar window */

HDC hDC;                    /* Private GDI Device Context of the main window */
HGLRC hRC;                  /* Permanent Rendering Context  of the main window */

HBITMAP hSplashBMP;         /* Bitmap will store the splash image */
HDC hMemoryDC;              /* Memory Device Context to store the splash image */
HDC hSplashDC;              /* Memory Device Context of the actual splash window */

long BitmapWidth;           /* The width of the bitmap */
long BitmapHeight;          /* The height of the bitmap */

char *WindowClassName;      /* Name for the Main Window Class */
char *SplashClassName;      /* Name for the Splash Window Class */

BOOL Paused;                /* Is the simulation paused */

RECT MainWindowRect;        /* Current Main Window's Rectangle */

Line MouseLine;             /* Current Drag Line */

Trajectory Traj;            /* Current Trajectory */
BOOL TrajCheck;             /* Is the trajectory checkbox checked? If yes Show trajectory */

int LbdMouse;               /* Left mouse button down count */
int LbuMouse;               /* Left mouse button up count */

BOOL LeftButtonDown;        /* Is Left mouse button down */
BOOL RightButtonDown;       /* Is Right mouse button down */

BOOL IsToolHidden;          /* Is the toolbar hidden */

BOOL Rbdbc;                 /* Right button double clicked? */

BOOL IsStarting;            /* When Left mouse button has been pressed once this will become false */

double mass;                 /* The last retrieved mass setting from the user */
float radius;               /* The last retrieved radius setting from the user */
Color colour;               /* The last retrieved colour setting from the user */

Point MousePos;             /* The last retrieved Mouse position from the user */

/* Initialize Window.c file */
BOOL InitWindow(HINSTANCE, char*, char*, RECT);

/* Creates the splash window of NBS */
BOOL CreateSplashWindow(HINSTANCE);

/* Creates the main window of NBS */
BOOL CreateMainWindow(HINSTANCE);

/* These functions are called by the Windows function DispatchMessage() and are the processors of all the commands received by the specific window */
LRESULT CALLBACK SplashProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MainProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ToolDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK GenHlpDlgProc(HWND, UINT, WPARAM, LPARAM);

/* Returns a pointer to the Handler of the Main Window */
HWND* GetMainHWND();

/* Returns a pointer to the Handler of the Toolbar Window */
HWND* GetToolHWND();

/* Returns a pointer to the device context */
HDC* GetMainDC();

/* Returns a pointer to the rendering context */
HGLRC* GetMainRC();

double getPanX();

double getPanY();

double getZoom();

/* Sets the title of the window which has the handler HWND passed in */
void SetTitle(HWND, char*);

/* Returns the rectangle of the client area */
RECT getWindowSize();

/* Returns the last retrieved value of the Mouse drag line */
Line getCurrDragLine();

/* Returns Whether the simulation is paused */
BOOL IsPaused();

/* Returns if trajectorization is to be done */
BOOL IsTraj();

/* Returns the last calculated trajectory of the current state of the mouse drag line */
Trajectory* GetWinTrajectory();

double ConvertRange(double, double, double, double, double);

/* Get global vars mass, radius and colour */
void GetCurrentMass(int);
void GetCurrentRadius(int);
void GetCurrentColour(int);

/* Return mass radius and colour */
float getMass();
float getRadius();
Color getColor();

/* Clears the Trajectory array */
void ClearTrajArr();

/* Gets text from id in hwnd */
void GetText(HWND, int);

/* Sets text of an id of hwnd */
void SetText(HWND, int, char *);

double getDistance();

/* Gets number from an id of hwnd */
BOOL GetNumber(HWND, int, BOOL, int *);
