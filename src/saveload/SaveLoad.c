#include "SaveLoad.h"

void SaveSim(Body *bArr, int size, HWND hwnd)
{
    OPENFILENAME ofn;
    char flName[MAX_PATH] = "";
    int i = 0;

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "N Body Simulator Save Files (*.nbs)\0*.nbs\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = flName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "nbs";

    if(GetSaveFileName(&ofn))
    {
        FILE *file=fopen(flName, "wb");
        Body b;
        for(i=0;i<size;i++)
        {
            b=bArr[i];
            fwrite(&b, sizeof(b), 1, file);
        }
        fclose(file);
    }

}

void LoadSim(HWND hwnd)
{
    OPENFILENAME ofn;
    char flName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "N Body Simulator Save Files (*.nbs)\0*.nbs\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = flName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "nbs";

    if(GetOpenFileName(&ofn))
    {
        FILE *file=fopen(flName, "rb");
        Body b;
        resetSim();
        while(!feof(file))
        {
                fread(&b, sizeof(b), 1, file);
                if(!feof(file))
                {
                    addBody(b);
                }
        }
        fclose(file);
    }
}
