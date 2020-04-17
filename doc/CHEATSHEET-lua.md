# Kaluimn - cheatsheet (Lua)

## Window

```lua
-- Returnes window width.
getWindowX()
-- Returnes window height.
getWindowY()
-- Changes window title.
setWindowTitle(title)
```

## Initialisation / Termination

```lua
-- Hides cursor, clears screen and sets window title.
init(title)
-- Shows cursor, restores default color and clears screen.
term()
```

## Canvas

```lua
-- Initialize canvas. Filled whith single color.
initCanvas(width, height, r, g, b)
-- Fills canvas with single color.
cleanCanvas(Color COLOR)
-- Dislays canvas content to the console. Only updates changed pixles.
displayCanvas()
```

## Shapes

```lua
-- Draw single pixel.
drawPixel(x, y, r, g, b, a)
-- Draw straight line.
drawLine(x1, y1, x2, y2, r, g, b, a, width)
-- Draw circle. Empty.
drawCircle(x1, y1, x2, y2, radius, r, g, b, a, width)
-- Draw circle. Filled.
drawCircleFill(x1, y1, x2, y2, radius, r, g, b, a)
-- Draw graph. (e.g. y = x)
drawGraph(graph, r, g, b, a, width)
```

## Textures

```lua
-- Returnes texture. PNG.
newTexture(path)
-- Draw texture.
drawTexture(texture, x, y)
```