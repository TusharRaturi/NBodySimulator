#include "NBS.h"

char *LogString = NULL;
MSG messages;            /* Here messages to the application are saved */

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    RECT WinSize;
    LogString = (char*)malloc((MAX_LOG - 12) * sizeof(char));

    printf("StartTime : %s\n", getReadableTime());

    printf("Initializing Timer...\n", getReadableTime());
    if(initTimer())
        printf("Timer Initialized.\n", getReadableTime());
    else
    {
        HandleException(NULL, "Could not initialize timer", "Error");
    }

    printf("Initializing Logger...\n", getReadableTime());
    if(initLogger())
        printf("Logger Initialized.\n", getReadableTime());
    else
    {
        HandleException(NULL, "Could not initialize logger", "Error");
    }

    Log("Initializing Application...");

    initMP3Player();

    WinSize.left = 0l;
    WinSize.top = 0l;
    WinSize.right = WIN_WIDTH;
    WinSize.bottom = WIN_HEIGHT;

    if(!InitWindow(hThisInstance, "NBSSIM", "NBSSPLASH", WinSize))
    {
        Log("Could not create the window.");
        HandleException(NULL, "Could not create the window.", "Error");
        return -1;
    }

    if(!initOGL(WinSize))
    {
        Log("Could not Initialize OpenGL.");
        HandleException(NULL, "Could not Initialize OpenGL.", "Error");
        return -1;
    }

    if(!initSimulator(MaximumBodies))
    {
        Log("Could not Initialize the Simulator.");
        HandleException(NULL, "Could not Initialize the Simulator.", "Error");
        return -1;
    }

    Log("Application Initialized.");

    run(*(GetMainHWND()), *(GetMainDC()), &messages, hThisInstance);

    /* The program's return-value is :- the value that PostQuitMessage(int) gave */
    return messages.wParam;
}

void run(HWND hwnd, HDC hDC, MSG *messages, HINSTANCE hThisInstance)
{
    char winTitle[27];
    double frameTime = 1.0/FRAME_RATE;

    sprintf(LogString, "%s%f", "Frames Per Second: ", FRAME_RATE);
    Log(LogString);
    //Started running
    boolint isRunning = true;
    double frames = 0, frameCounter = 0, lastTime = 0, unprocessedTime = 0;

    Log("Application Started.");

    lastTime = getNanoTime();
    unprocessedTime = 0;

    while(isRunning)
    {
        boolint render = false;

        double startTime = getNanoTime();
        double passedTime = startTime - lastTime;

        lastTime = startTime;
        unprocessedTime += passedTime;
        frameCounter += passedTime;

        while(unprocessedTime > frameTime)
        {
            render = true;
            unprocessedTime -= frameTime;

            if(!IsPaused())
                updateSimulator(frameTime, getCollisionType(), getDistance());

            if(TrajCheck && !(getBodyArrSize() <=25))
            {
                //Too many bodies....Can't do trajectory calculation.
            }
            else if(TrajCheck && LeftButtonDown && !Rbdbc)
            {
                GetCurrentMass(0);
                GetCurrentRadius(0);
                GetCurrentColour(0);
                SetWinTrajectory(getTrajectory(getMass(), getRadius(), getCurrDragLine(), getColor(), getDistance(), getCollisionType()));
            }

            if(frameCounter >= 1.0)
            {
                sprintf(winTitle, "%s FPS = %f", WIN_TITLE, frames);
                setTitle(hwnd, winTitle);
                frames = frameCounter = 0;
            }
        }
        if(render)
        {
            //render
            DrawGLScene(getBodyArrPtr(), getCurrDragLine(), getBodyArrSize(), GetWinTrajectory(), DrawAccuracy, IsTraj(), getPanX(), getPanY());
            DrawOGL(hDC);

            frames++;
        }
        else
        {
            nanoSleep(1);
        }

        if (PeekMessage(&(*messages),NULL,0,0,PM_REMOVE))// Is There A Message Waiting?
		{
			if ((*messages).message == WM_QUIT)// Have We Received A Quit Message?
			{
				isRunning=false;// If So done=TRUE
			}
			else if(!IsDialogMessage((*GetToolHWND()), &(*messages)))// If Not, Deal With Window Messages if it is not of the toolbar else nothing
			{
			    /* Translate virtual-key messages into character messages */
				TranslateMessage(&(*messages));
				/* Send(Dispatch) message to WindowProcedure */
				DispatchMessage(&(*messages));
			}
		}
    }

    Log("Application Ended.");

    Log("Starting Clean Up.");
}
