//----------------------------------------------------------------------------//
// drawing functions
//----------------------------------------------------------------------------//


#ifndef DRAW
#define DRAW

int checkColor(const int COLOR);
int colorCode(const char COLOR[]);
int setColor(const int FOREGROUND, const int BACKGROUND);
int drawTexture(const int X, const int Y, const char PATH[]);
int drawPixel(const int X, const int Y, const int COLOR);

#endif