# Kaluimn - cheatsheet (C)

## Types

```c
// Color type. Used in canvas and textures.
struct Color{
    int r;
    int g;
    int b;
    int a;
};
// Canvas type. Stores current and previous states. Not Accesible by user. Global.
struct Canvas{
    int width;
    int height;
    Color *pixels;
    Color *previousPixels;
};
// Texture type. Returned to user.
struct Texture{
    int width;
    int height;
    Color *pixels;
};
// Sound type.
struct Sound{
    char *path
};
```

## Window

```c
// Returnes window width.
int getWindowX();
// Returnes window height.
int getWindowY();
// Changes window title.
void setWindowTitle(char TITLE[]);
```

## Initialisation / Termination

```c
// Hides cursor, clears screen and sets window title.
void init(char TITLE[]);
// Shows cursor, restores default color and clears screen.
void term();
```

## Canvas

```c
// Initialize canvas. Filled whith single color.
void initCanvas(int WIDTH, int HEIGHT, Color COLOR);
// Fills canvas with single color.
void cleanCanvas(Color COLOR);
// Dislays canvas content to the console. Only updates changed pixles.
void displayCanvas();
```

## Shapes

```c
// Draw single pixel.
void drawPixel(int X, int Y, Color COLOR);
// Draw straight line.
void drawLine(int X1, int Y1, int X2, int Y2, Color COLOR, int WIDTH);
// Draw circle. Empty.
void drawCircle(int X1, int Y1, int X2, int Y2, int R, Color COLOR, int WIDTH);
// Draw circle. Filled.
void drawCircleFill(int X1, int Y1, int X2, int Y2, int R, Color COLOR);
// Draw graph. (e.g. y = x)
void drawGraph(char GRAPH[], Color COLOR, int WIDHT);
```

## Textures

```c
// Returnes texture. PNG.
Texture newTexture(char PATH[]);
// Draw texture.
void drawTexture(Texture TEXTURE, int X, int Y);
```

