# kaluimn cheatsheet

## Types

```c
struct Color{int r; int g; int b; int a};

struct Pixel{int r; int g; int b; int a};

struct Canvas{int width; int height; Pixel *pixels; Pixel *previousPixels};

struct Texture{int width; int height; Pixel *pixels};

struct Sound{char *path};
```

## Canvas

```c
int initCanvas(int WIDTH, int HEIGHT, int R, int G, int B);

int cleanCanvas(int R, int G, int B);

int display();
```

## Draw

```c

```
