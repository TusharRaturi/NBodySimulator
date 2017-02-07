#include "ExceptionHandle.h"

void HandleException(HWND hwnd, char *contents, char *title)
{
    MessageBox(hwnd, contents, title, MB_OK|MB_ICONERROR);
}
