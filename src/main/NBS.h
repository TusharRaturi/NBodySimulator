#include "../window/NBSWin.h"
#define boolint int
#define true 1
#define false 0

//If V-Sync is enabled then frame rate is already capped but still...
#define FRAME_RATE 60.0

#define MaximumBodies 500

#define DrawAccuracy 0.1

/* Main function for Windows GUI based programs */
int WINAPI WinMain (HINSTANCE, HINSTANCE, LPSTR, int);

void run(HWND, HDC, MSG*, HINSTANCE);
