#include "music.h"

MP3Player mpp[3];
int curr;

int initMP3Player()
{
    char tmp[100];
    mpp[0].music = (char*)malloc(100*sizeof(char));
    mpp[1].music = (char*)malloc(100*sizeof(char));
    mpp[2].music = (char*)malloc(100*sizeof(char));
    mpp[0].music = "music/Moon_Garden.mp3";
    mpp[1].music = "music/Autumn_Nights.mp3";
    mpp[2].music = "music/Mercury.mp3";

    sprintf(tmp, "open \"%s\" type mpegvideo alias mp3%d", mpp[0].music, 0);
    mciSendString(tmp, NULL, 0, NULL);
    sprintf(tmp, "open \"%s\" type mpegvideo alias mp3%d", mpp[1].music, 1);
    mciSendString(tmp, NULL, 0, NULL);
    sprintf(tmp, "open \"%s\" type mpegvideo alias mp3%d", mpp[2].music, 2);
    mciSendString(tmp, NULL, 0, NULL);

    curr = 0;
}

void playFromPlayList(HWND hwnd)
{
    char tmp[100];

    if(curr > 2)
        curr = 0;

    sprintf(tmp, "play mp3%d notify from 0", curr);
    mciSendString(tmp, NULL, 0, hwnd);

    curr++;
}

int addSound(char *in)
{
    char tmp[100];
    int x=0;

    sprintf(tmp, "open \"%s\" type mpegvideo alias snd%d", in, x);
    mciSendString(tmp, NULL, 0, NULL);

    return x;
}

void playSound(int snd)
{
    char tmp[100];

    sprintf(tmp, "play snd%d from 0", snd);
    mciSendString(tmp, NULL, 0, NULL);
}
