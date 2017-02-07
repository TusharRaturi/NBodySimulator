#include "NBSWin.h"

double pan_x = 0.0;
double pan_y = 0.0;

int collT = COLLISION_DESTROY;
double unitT = UNIT_METER;

Point va;
Point vb;

double panLineX=0.0, panLineY=0.0;

int MiddleButtonDown=0;

Line PanLine;

BOOL InitWindow(HINSTANCE h, char *WinClsN, char *SplClsN, RECT WinSize)
{
    hDC = NULL;
    hRC = NULL;

    hSplashBMP = NULL;

    hMemoryDC = NULL;
    hSplashDC = NULL;

    BitmapWidth = BitmapHeight = 0l;

    WindowClassName = WinClsN;
    SplashClassName = SplClsN;

    Paused = FALSE;

    MainWindowRect = WinSize;

    MouseLine = getNewLine(getNewPoint(0, 0), getNewPoint(0, 0));

    Traj.clrr = getNewColor(0.0, 0.0, 0.0, 0.0);
    Traj.size = 0;

    TrajCheck = FALSE;

    LbdMouse = 0;
    LbuMouse = 0;

    LeftButtonDown = FALSE;
    RightButtonDown = FALSE;

    IsToolHidden = FALSE;

    Rbdbc = FALSE;

    IsStarting = TRUE;

    mass = 0.0;
    radius = 0.0;
    colour = getNewColor(0.0, 0.0, 0.0, 0.0);

    MousePos = getNewPoint(0, 0);

    return CreateSplashWindow(h) && CreateMainWindow(h);
}

BOOL CreateSplashWindow(HINSTANCE hIn)
{
    WNDCLASSEX SWindow; /* Window class of the splash window */
    RECT DesktopRect;   /* Will store the desktop rectangle */
    BITMAP Bitmap;      /* The splash img. */
    HWND hSplashWnd;    /* The handle will not be needed in the future so it is not global. */

    // Get the size of desktop
    GetWindowRect(GetDesktopWindow(), &DesktopRect);

    // Fill up the Window class structure
    SWindow.cbSize           = sizeof(WNDCLASSEX);
    SWindow.style            = 0;
    SWindow.lpfnWndProc      = (WNDPROC)SplashProc;
    SWindow.cbClsExtra       = 0;
    SWindow.cbWndExtra       = 0;
    SWindow.hInstance        = hIn;
    SWindow.hIcon            = NULL;
    SWindow.hIconSm          = NULL;
    SWindow.hCursor          = LoadCursor(NULL, IDC_ARROW);
    SWindow.hbrBackground    = NULL;
    SWindow.lpszMenuName     = NULL;
    SWindow.lpszClassName    = SplashClassName;

    // Register the window
    if (!RegisterClassEx(&SWindow))
    {
        Log("Failed To Register The Splash Window Class.");
        HandleException(NULL, "Failed To Register The Splash Window Class.", "Error");
        return FALSE;
    }

    // Load the bitmap and get it into the bitmap variable
    hSplashBMP = LoadBitmap(hIn, MAKEINTRESOURCE(BITMAP_ID));
    GetObject(hSplashBMP, sizeof(BITMAP), &Bitmap);

    // Setup the bitmap size variables
    BitmapWidth = Bitmap.bmWidth;
    BitmapHeight = Bitmap.bmHeight;

    // Create the window.
    if(!(hSplashWnd = CreateWindowEx(
    0,
    SplashClassName,
    "NBS Splash",
    WS_POPUP,
    (DesktopRect.right - BitmapWidth) / 2,
    (DesktopRect.bottom - BitmapHeight) / 2,
    Bitmap.bmWidth,
    Bitmap.bmHeight,
    NULL,
    NULL,
    hIn,
    NULL)))
    {
        Log("Failed To Create The Splash Window.");
        HandleException(NULL, "Failed To Create The Splash Window.", "Error");
        return FALSE;
    }

    // Fill the Device context of splash
    hSplashDC = GetDC(hSplashWnd);

    // Fill splash device context
    hMemoryDC = CreateCompatibleDC(hSplashDC);
    SelectObject(hMemoryDC, (HGDIOBJ)hSplashBMP);

    // Show Splash Window
    ShowWindow(hSplashWnd, SW_SHOW);
    //Update Splash Window
    UpdateWindow(hSplashWnd);

    return TRUE;
}

BOOL CreateMainWindow(HINSTANCE hIn)
{
    WNDCLASSEX MWindow;         /* Data structure for the window class */
    HICON icon;                 /* The icon for the window */
    int iFormat;
    PIXELFORMATDESCRIPTOR pfd;  /* This informs the system how we are going to use the DC. In other words, the features of OpenGl we need active */

    // Load the icon
    icon = LoadIcon(hIn, MAKEINTRESOURCE(ICON_ID));

    // Fill the Window structure
    MWindow.hInstance = hIn;
    // Class Name of the window class
    MWindow.lpszClassName = WindowClassName;
    // Function called by windows
    MWindow.lpfnWndProc = MainProc;
    // Catch double clicks and own the Device Context(for opengl)
    MWindow.style = CS_DBLCLKS | CS_OWNDC;
    // Size of window structure
    MWindow.cbSize = sizeof (WNDCLASSEX);

    // Use default icon and mouse-pointer
    MWindow.hIcon = icon;
    MWindow.hIconSm = icon;
    MWindow.hCursor = LoadCursor(NULL, IDC_ARROW);
    // Set the Resource Menu
    MWindow.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    // No extra bytes after the window class
    MWindow.cbClsExtra = 0;
    // Structure or the window instance
    MWindow.cbWndExtra = 0;
    // Use Windows's default colour as the background of the window
    MWindow.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    // Register the window class, and if it fails quit the program
    if (!RegisterClassEx(&MWindow))
    {
        Log("Could not register window class.");
        HandleException(NULL, "Could not register window class.", "Error");
        return FALSE;
    }

    // Adjust Window To True Requested Size
    AdjustWindowRectEx(&MainWindowRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_ACCEPTFILES);

    //  Create the program after registering the class
    if(!(hMainWindow = CreateWindowEx (
           WS_EX_APPWINDOW | WS_EX_ACCEPTFILES,     /* Extended possibilities for variation */
           WindowClassName,     /* Classname */
           WIN_TITLE,           /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           MainWindowRect.right,           /* The programs width */
           MainWindowRect.bottom,          /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hIn,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           )))
    {
        Log("Could Not Create Window.");
        HandleException(NULL, "Could Not Create Window.", "Error");
        return FALSE;
    }

    // Create Device Context
    if(!(hDC = GetDC(hMainWindow)))
    {
        Log("Could Not Create Device Context.");
        HandleException(NULL, "Could Not Create Device Context.", "Error");
        return FALSE;
    }

    // Zero out the memory(initialize)
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    // Picks the best match for what we requested
    if(!(iFormat = ChoosePixelFormat(hDC, &pfd)))
    {
        Log("Can't Find A Suitable PixelFormat.");
        HandleException(NULL, "Can't Find A Suitable PixelFormat.","Error");
        return FALSE;
    }

    if(!SetPixelFormat(hDC, iFormat, &pfd))
    {
        Log("Can't Set The PixelFormat.");
        HandleException(NULL, "Can't Set The PixelFormat.", "Error");
        return FALSE;
    }

    // Create Render Context
    if(!(hRC = wglCreateContext(hDC)))
    {
        Log("Could Not Create OpenGL Rendering Context.");
        HandleException(NULL, "Could Not Create OpenGL Rendering Context.", "Error");
        return FALSE;
    }

    // Try To Activate The Rendering Context
    if(!wglMakeCurrent(hDC, hRC))
	{
	    Log("Could not Activate The GL Rendering Context.");
		HandleException(NULL, "Could not Activate The GL Rendering Context.", "Error");
		return FALSE;
	}

	// SEt the icon
    SendMessage(hMainWindow, WM_SETICON, ICON_ID, (LPARAM)icon);

    return TRUE;
}

LRESULT CALLBACK SplashProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_ERASEBKGND:
            // Erase background
            BitBlt((HDC)wParam, 0, 0, BitmapWidth, BitmapHeight, hMemoryDC, 0, 0, SRCCOPY);
        break;

        // If pressed a key or a mouse button
        case WM_CHAR:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            //Close the splash and activate the main
            DeleteObject(hSplashBMP);
            ReleaseDC(hwnd, hSplashDC);
            ReleaseDC(hwnd, hMemoryDC);
            DestroyWindow(hwnd);
            ShowWindow(hToolWindow, SW_SHOW);
            ShowWindow(hMainWindow, SW_SHOW);
            UpdateWindow(hMainWindow);
            SetForegroundWindow(hMainWindow);
            SetForegroundWindow(hToolWindow);
            playFromPlayList(hMainWindow);
            break;

        default:
            return (DefWindowProc(hwnd, Message, wParam, lParam));
    }
    return 0;
}

LRESULT CALLBACK MainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Handle messages
    switch(message)
    {
        case WM_CREATE:
			if(!(hToolWindow = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR), hwnd, ToolDlgProc)))
			{
				Log("CreateDialog returned NULL");
				HandleException(hwnd, "CreateDialog returned NULL", "Warning!");
			}
			else
            {
                UpdateWindow(hToolWindow);
            }
        break;

        case WM_COMMAND:

            switch(LOWORD(wParam))
            {
                case IDM_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;

                case IDM_TOOLBAR:
                    if(!IsToolHidden)
                    {
                        ShowWindow(hToolWindow, SW_HIDE);
                        CheckMenuItem(GetMenu(hwnd), IDM_TOOLBAR, MF_UNCHECKED);
                        IsToolHidden = TRUE;
                    }
                    else
                    {
                        ShowWindow(hToolWindow, SW_SHOW);
                        CheckMenuItem(GetMenu(hwnd), IDM_TOOLBAR, MF_CHECKED);
                        IsToolHidden = FALSE;
                    }
                break;

                case IDM_ABOUT:
                    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutDlgProc);
                break;

                case IDM_GENERAL_HELP:
                    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, GenHlpDlgProc);
                break;

                case IDM_SAVE:
                    SaveSim(getBodyArrPtr(), getBodyArrSize(), hwnd);
                break;

                case IDM_LOAD:
                    LoadSim(hwnd);
                break;

                case IDM_NEW:
                    ClearTrajArr();
					resetSim(MaxBodies);
					resetCam();
                break;
            }
        break;

        case WM_SIZE:
            GetClientRect(hwnd, &MainWindowRect);
            ResizeCanvas(MainWindowRect);
        break;

        case WM_MOUSEMOVE:
            {
                switch(wParam)
                {
                    case MK_RBUTTON:
                        {
                            char *tmp = (char*)malloc(20*sizeof(char));
                            sprintf(tmp, "Sizer - (%d, %d)", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                            SetText(hToolWindow, IDC_SIZER, tmp);
                            free(tmp);
                            MouseLine.b.x = GET_X_LPARAM(lParam) - (int)pan_x;
                            MouseLine.b.y = GET_Y_LPARAM(lParam) - (int)pan_y;
                        }
                    break;

                    case MK_LBUTTON:
                        MouseLine.b.x = GET_X_LPARAM(lParam) - (int)pan_x;
                        MouseLine.b.y = GET_Y_LPARAM(lParam) - (int)pan_y;
                    break;

                    case MK_MBUTTON:
                        PanLine.b.x = GET_X_LPARAM(lParam) + panLineX;
                        PanLine.b.y = GET_Y_LPARAM(lParam) + panLineY;
                        pan(-(PanLine.a.x - PanLine.b.x), -(PanLine.a.y - PanLine.b.y));
                    break;
                }
            }
        break;

        case WM_LBUTTONDOWN:
            LeftButtonDown = TRUE;
            SetCapture(hwnd);
            LbdMouse++;
            if(LbdMouse-1 > LbuMouse)
            {
                MouseLine = getNewLine(getNewPoint(0, 0), getNewPoint(0, 0));
                LbdMouse = LbuMouse;
            }
            MouseLine.a.x = GET_X_LPARAM(lParam) - pan_x;
            MouseLine.a.y = GET_Y_LPARAM(lParam) - pan_y;
            MouseLine.b.x = GET_X_LPARAM(lParam) - pan_x;
            MouseLine.b.y = GET_Y_LPARAM(lParam) - pan_y;
            va = MouseLine.a;
        break;

        case WM_LBUTTONUP:
            {
                if(Rbdbc)
                {
                    Rbdbc = FALSE;
                    LeftButtonDown = FALSE;
                }
                else
                {
                    Vec2d v;
                    Point tmpPoint;
                    LbuMouse++;
                    if(!LbdMouse || (Rbdbc && LbdMouse < LbuMouse))
                    {
                        MouseLine = getNewLine(getNewPoint(0, 0), getNewPoint(0, 0));
                        LbuMouse = LbdMouse;
                    }
                    else
                    {
                        MouseLine.b = getNewPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

                        GetCurrentMass(1);

                        GetCurrentRadius(1);

                        GetCurrentColour(1);

                        tmpPoint.x = MouseLine.b.x - pan_x;
                        tmpPoint.y = MouseLine.b.y - pan_y;

                        v = getNewVec2d((va.x - tmpPoint.x)/100.0, (va.y - tmpPoint.y)/100.0);

                        addBody(getNewBody(tmpPoint, v, mass, radius, colour));
                        IsStarting = FALSE;
                    }
                    LeftButtonDown = FALSE;
                }
                MouseLine = getNewLine();
                ReleaseCapture();
            }
        break;

        case WM_RBUTTONUP:
            {
                char *tmp = (char*)malloc(20*sizeof(char));
                sprintf(tmp, "Sizer - (%d, %d)", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                SetText(hToolWindow, IDC_SIZER, tmp);
                free(tmp);
                RightButtonDown = FALSE;
                MouseLine.b.x = GET_X_LPARAM(lParam) - (int)pan_x;
                MouseLine.b.y = GET_Y_LPARAM(lParam) - (int)pan_y;
            }
        break;

        case WM_RBUTTONDOWN:
            {
                char *tmp = (char*)malloc(20*sizeof(char));
                sprintf(tmp, "Sizer - (%d, %d)", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                SetText(hToolWindow, IDC_SIZER, tmp);
                RightButtonDown = TRUE;
                MouseLine.b.x = GET_X_LPARAM(lParam) - pan_x;
                MouseLine.b.y = GET_Y_LPARAM(lParam) - pan_y;
            }
        break;

        case WM_MBUTTONDOWN:
            MiddleButtonDown = 1;
            PanLine.a.x = GET_X_LPARAM(lParam);
            PanLine.a.y = GET_Y_LPARAM(lParam);
            PanLine.b.x = GET_X_LPARAM(lParam);
            PanLine.b.y = GET_Y_LPARAM(lParam);
        break;

        case WM_MBUTTONUP:
            MiddleButtonDown = 0;
            panLineX = pan_x;
            panLineY = pan_y;
        break;

        /*case WM_MOUSEWHEEL:
            {
                int w = GET_WHEEL_DELTA_WPARAM(wParam);
                zoom((double)w);
            }
        break;*/

        case WM_RBUTTONDBLCLK:
            if(!IsStarting)
            {
                if(LeftButtonDown)
                    Rbdbc = 1;
            }
        break;

        case WM_CLOSE:
            if(MessageBox(hwnd, "Are you sure you want to quit the NBS?", "Quit?", MB_YESNO | MB_ICONQUESTION) == IDYES)
                DestroyWindow(hwnd);
        break;

        case WM_DESTROY:
            DestroyWindow(hToolWindow);
            PostQuitMessage(0);       // send a WM_QUIT to the message queue
        break;

        case MM_MCINOTIFY:
            playFromPlayList(hwnd);
        break;

        default:                      // for messages that we don't deal with
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

int getCollisionType()
{
    return collT;
}

double ConvertRange(double originalStart, double originalEnd, double newStart, double newEnd, double value)
{
    double scale = (newEnd - newStart) / (originalEnd - originalStart);
    return newStart + ((value - originalStart) * scale);
}

double getDistance()
{
    return unitT;
}

BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
        case WM_CREATE:
            {
                LPARAM bounds = MAKELPARAM(0, 100);
                SendMessage(GetDlgItem(hwnd, RED), TBM_SETRANGE, TRUE, bounds);
                SendMessage(GetDlgItem(hwnd, GREEN), TBM_SETRANGE, TRUE, bounds);
                SendMessage(GetDlgItem(hwnd, BLUE), TBM_SETRANGE, TRUE, bounds);
            }
        break;

        case WM_INITDIALOG:
            SendMessage(GetDlgItem(hwnd, IDC_COLLISION_DROP), CB_ADDSTRING, 0, (LPCTSTR)"Destroy");
            SendMessage(GetDlgItem(hwnd, IDC_COLLISION_DROP), CB_ADDSTRING, 0, (LPCTSTR)"Merge");
            SendMessage(GetDlgItem(hwnd, IDC_COLLISION_DROP), CB_ADDSTRING, 0, (LPCTSTR)"Merge&Destroy");
            SendMessage(GetDlgItem(hwnd, IDC_COLLISION_DROP), CB_SETCURSEL, 0, 0);

            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1nm");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1mim");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1mm");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1cm");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1m");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1km");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 2.5e+5m");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 5.0e+6m");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 2.0e+7m");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 7.5e+7m");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1.5e+8m");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1ld");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1au");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1ly");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_ADDSTRING, 0, (LPCTSTR)"1 unit = 1pa");
            SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_SETCURSEL, 0, 0);
        break;

		case WM_COMMAND:

			switch(LOWORD(wParam))
			{
				case IDC_PAUSE:
					Paused = !Paused;
					if(Paused)
                        SetText(hwnd, IDC_PAUSE, "Play");
                    else
                        SetText(hwnd, IDC_PAUSE, "Pause");
                    ClearTrajArr();
                break;
				case IDC_RESET:
				    ClearTrajArr();
					resetSim(MaxBodies);
					resetCam();
                break;
                case IDC_TRAJ:
                    TrajCheck = !TrajCheck;
                    if(TrajCheck)
                        CheckDlgButton(hwnd, IDC_TRAJ, BST_CHECKED);
                    else
                        CheckDlgButton(hwnd, IDC_TRAJ, BST_UNCHECKED);

                case IDC_COLLISION_DROP:
                    {
                        int tmp = 0;
                        switch(HIWORD(wParam))
                        {
                            case CBN_SELCHANGE:
                                tmp = SendMessage(GetDlgItem(hwnd, IDC_COLLISION_DROP), CB_GETCURSEL, 0, 0);
                                if(tmp == 0)
                                    collT = COLLISION_DESTROY;
                                else if(tmp == 1)
                                    collT = COLLISION_MERGE;
                                else if(tmp == 2)
                                    collT = COLLISION_MERGE_DESTROY;
                                else
                                    collT = COLLISION_DESTROY;
                            break;
                        }
                    }
                break;

                case IDC_UNITS_DROP:
                    {
                        int tmp = 0;
                        switch(HIWORD(wParam))
                        {
                            case CBN_SELCHANGE:
                                tmp = SendMessage(GetDlgItem(hwnd, IDC_UNITS_DROP), CB_GETCURSEL, 0, 0);
                                if(tmp == 0)
                                    unitT = UNIT_NANOMETER;
                                else if(tmp == 1)
                                    unitT = UNIT_MICROMETER;
                                else if(tmp == 2)
                                    unitT = UNIT_MILLIMETER;
                                else if(tmp == 3)
                                    unitT = UNIT_CENTIMETER;
                                else if(tmp == 4)
                                    unitT = UNIT_METER;
                                else if(tmp == 5)
                                    unitT = UNIT_KILOMETER;
                                else if(tmp == 6)
                                    unitT = UNIT_TEST0;
                                else if(tmp == 7)
                                    unitT = UNIT_TEST1;
                                else if(tmp == 8)
                                    unitT = UNIT_TEST2;
                                else if(tmp == 9)
                                    unitT = UNIT_TEST3;
                                else if(tmp == 10)
                                    unitT = UNIT_TEST4;
                                else if(tmp == 11)
                                    unitT = UNIT_LUNAR_DISTANCE;
                                else if(tmp == 12)
                                    unitT = UNIT_ASTRONOMICAL_UNIT;
                                else if(tmp == 13)
                                    unitT = UNIT_LIGHT_YEAR;
                                else if(tmp == 14)
                                    unitT = UNIT_PARSEC;
                            break;
                        }
                    }
                break;
			}
            break;

        case WM_DESTROY:
            DestroyWindow(hwnd);
            PostQuitMessage(0);
        break;

		default:
			return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:

        return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwnd, IDOK);
                break;
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK GenHlpDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:

        return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwnd, IDOK);
                break;
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}
/*
void zoom(double factor)
{
    if(zoom_k <= scale)
    {
        zoom_k = scale;
        if(factor <= 0.0)
            return;
    }
    factor /= 120;
    zoom_k += scale * factor;
}
*/
void pan(double dx, double dy)
{
    pan_x = dx;
    pan_y = dy;
}

void resetCam()
{
    panLineX = pan_x = 0.0;
    panLineY = pan_y = 0.0;
}

HWND* GetMainHWND()
{
    return &hMainWindow;
}

HWND* GetToolHWND()
{
    return &hToolWindow;
}

HDC* GetMainDC()
{
    return &hDC;
}

HGLRC* GetMainRC()
{
    return &hRC;
}

double getPanX()
{
    return pan_x;
}

double getPanY()
{
    return pan_y;
}
/*
double getZoom()
{
    return zoom_k;
}
*/
void setTitle(HWND hwnd, char *title)
{
    SetWindowText(hwnd, title);
}

RECT getWindowSize()
{
    return MainWindowRect;
}

Line getCurrDragLine()
{
    return MouseLine;
}

int IsPaused()
{
    return Paused;
}

BOOL IsTraj()
{
    return TrajCheck;
}

void SetWinTrajectory(Trajectory *traj)
{
    Traj = *traj;
}

Trajectory* GetWinTrajectory()
{
    return &Traj;
}

void GetCurrentMass(int log)
{
    int m1 = 0, m2 = 0;
    if(GetNumber(hToolWindow, IDC_MASS_A, FALSE, &m1) == TRUE && GetNumber(hToolWindow, IDC_MASS_B, TRUE, &m2) == TRUE)

    mass = (double)(m1) * pow(10, m2);

    else
    {
        if(log)
        {
            Log("Unable To Read Mass. Resetting to Default.");
            SetText(hToolWindow, IDC_MASS_A, "1");
            SetText(hToolWindow, IDC_MASS_B, "11");
        }
        mass = 1e+11;
    }
    if(mass < 1000.0 || mass > 1e+40)
    {
        if(log)
        {
            Log("Mass is too divergent. Resetting to Default.");
            SetText(hToolWindow, IDC_MASS_A, "1");
            SetText(hToolWindow, IDC_MASS_B, "11");
        }
        mass = 1e+11;
    }
}

void GetCurrentRadius(int log)
{
    int r1 = 0, r2 = 0;

    if(GetNumber(hToolWindow, IDC_RADIUS_A, FALSE, &r1) == TRUE && GetNumber(hToolWindow, IDC_RADIUS_B, TRUE, &r2) == TRUE)
        radius = (float)(r1 * pow(10, r2));
    else
    {
        if(log)
        {
            Log("Unable To Read Radius. Resetting to Default.");
            SetText(hToolWindow, IDC_RADIUS_A, "5");
            SetText(hToolWindow, IDC_RADIUS_B, "0");
        }
        radius = 5.0;
    }
    if(radius < 5.0 || radius > 1000.0)
    {
        if(log)
        {
            Log("Radius is too divergent. Resetting to Default.");
            SetText(hToolWindow, IDC_RADIUS_A, "5");
            SetText(hToolWindow, IDC_RADIUS_B, "0");
        }
        radius = 5.0;
    }
}

void GetCurrentColour(int log)
{
    int p1 = 0, p2 = 0, p3 = 0;

    p1 = SendMessage(GetDlgItem(hToolWindow, RED), TBM_GETPOS, 0, 0);
    p2 = SendMessage(GetDlgItem(hToolWindow, GREEN), TBM_GETPOS, 0, 0);
    p3 = SendMessage(GetDlgItem(hToolWindow, BLUE), TBM_GETPOS, 0, 0);
    colour.r=p1/100.0;colour.g=p2/100.0;colour.b=p3/100.0;

    if(p1 + p2 + p3 < 30)
    {
        if(log)
        {
            Log("Colour is too dark. Resetting To Default");
            SendMessage(GetDlgItem(hToolWindow, RED), TBM_SETPOS, TRUE, 50);
            SendMessage(GetDlgItem(hToolWindow, GREEN), TBM_SETPOS, TRUE, 50);
            SendMessage(GetDlgItem(hToolWindow, BLUE), TBM_SETPOS, TRUE, 50);
        }
        colour.r=colour.g=colour.b=0.5;
    }
}

float getMass()
{
    return mass;
}

float getRadius()
{
    return radius;
}

Color getColor()
{
    return colour;
}

void ClearTrajArr()
{
    int i;
    if(Traj.points != NULL)
    {
        for(i = 0; i < Traj.size; i++)
            Traj.points[i] = getNewPoint(0, 0);
    }
}

void GetText(HWND hwnd, int id)
{
    int len = GetWindowTextLength(GetDlgItem(hwnd, id));
    if(len > 0)
    {
        int i;
        char* buf;

        buf = (char*)GlobalAlloc(GPTR, len + 1);
        GetDlgItemText(hwnd, id, buf, len + 1);

        return buf;
    }
}

void SetText(HWND hwnd, int id, char *str)
{
    SetDlgItemText(hwnd, id, str);
}

BOOL GetNumber(HWND hwnd, int id, BOOL bSigned, int *num)
{
    BOOL bSuccess;
    *num = GetDlgItemInt(hwnd, id, &bSuccess, bSigned);
    return bSuccess;
}
