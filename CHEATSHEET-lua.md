# Kaluimn - cheatsheet (Lua)

## Tables

* **texture{`width`, `height`, `data`}**  
	Contains texture information.
	* **`width`**  
		Texture width.
	* **`height`**  
		Texture height.
	* **`data`**  
		Array containing pixel information.  
		(`p1.r`, `p1.g`, `p1.b`, `p1.a`, `p2.r`, `p2.g`, `p2.b`, ...)

## Window

* **getWinX()**  
	Returns window width.

* **getWinY()**  
	Returns window height.

* **setWindowTitle(`title`)**  
	Changes window title.
	* **`title`**  
		Window title.

## Initialisation / Termination

* **init(`title`)**  
	Hides cursor, clears screen and sets window title.   
	***Call at start of program.***
	* **`title`**  
		Window title.

* **init()**  
	Shows cursor, restores default color and clears screen.  
	***Call at end of program.***

## Canvas

* **initCanvas(`width`, `height`, `r`, `g`, `b`)**  
	Initialize canvas. Filled whith single color.
	* **`width`**  
		Canvas width.
	* **`height`**  
		Canvas height.
	* **`r`**  
		Color red(0 ~ 255).
	* **`g`**  
		Color green(0 ~ 255).
	* **`b`**  
		Color blue(0 ~ 255).

* **cleanCanvas(`r`, `g`, `b`)**  
	Fills canvas with single color.
	* **`r`**  
		Color red(0 ~ 255).
	* **`g`**  
		Color green(0 ~ 255).
	* **`b`**  
		Color blue(0 ~ 255).

* **displayCanvas()**  
	Dislays canvas content to the console. Only updates changed pixles.

* **displayCanvasAll()**  
	Dislays canvas content to the console. Only all pixles.  
	***Very Slow.***


## Shapes



* **drawPixel(`x`, `y`, `r`, `g`, `b`, `a`)**  
	Draw single pixel.
	* **`x`**  
		X position.
	* **`y`**  
		Y position.
	* **`r`**  
		Color red(0 ~ 255).
	* **`g`**  
		Color green(0 ~ 255).
	* **`b`**  
		Color blue(0 ~ 255).

* **drawLine(`x1`, `y1`, `x2`, `y2`, `r`, `g`, `b`, `a`, `width`)**  
	Draw straight line.
	* **`x1`**  
		X position of first point.
	* **`y1`**  
		Y position of first point.
	* **`x2`**  
		X position of second point.
	* **`y2`**  
		Y position of second point.
	* **`r`**  
		Color red(0 ~ 255).
	* **`g`**  
		Color green(0 ~ 255).
	* **`b`**  
		Color blue(0 ~ 255).
	* **`width`**  
		Line width.

* **drawCircle(`x1`, `y1`, `radius`, `r`, `g`, `b`, `a`, `width`)**  
	Draw circle. Empty.
	* **`x`**  
		X center position.
	* **`y`**  
		Y center position.
	* **`radius`**  
		Rircle radius.
	* **`r`**  
		Color red(0 ~ 255).
	* **`g`**  
		Color green(0 ~ 255).
	* **`b`**  
		Color blue(0 ~ 255).
	* **`width`**  
		Line width.

* **drawCircleFill(`x1`, `y1`, `radius`, `r`, `g`, `b`, `a`)**  
	Draw circle. Filled.
	Draw circle. Empty.
	* **`x`**  
		X center position.
	* **`y`**  
		Y center position.
	* **`radius`**  
		Rircle radius.
	* **`r`**  
		Color red(0 ~ 255).
	* **`g`**  
		Color green(0 ~ 255).
	* **`b`**  
		Color blue(0 ~ 255).

* **drawGraph(`graph`, `r`, `g`, `b`, `a`, `width`)**  
	Draw graph.
	* **`graph`**  
		Graph equation(e.g. y = x^2 + 2x + 3).
	* **`r`**  
		Color red(0 ~ 255).
	* **`g`**  
		Color green(0 ~ 255).
	* **`b`**  
		Color blue(0 ~ 255).
	* **`width`**  
		Line width.

## Textures

* **newTexture(`path`)**  
	Returns texture.  
	***Only PNG files.***
	* **`path`**  
		Relative path to png file.

* **drawTexture(`texture`, `x`, `y`)**  
	Draw texture to canvas.
	* **`texture`**  
		Texture object(created with `newTexture()`).
	* **`x`**  
		X position(top-left corner).
	* **`y`**  
		Y position(top-left corner).

## Misc

* **waitForKeyPress()**  
	Pauses until `↵` has been pressed.  
	Same as `getchar()` in c.

* **getTime()**  
	Returns time since `init()` in seconds (micro second presicion).

* **getDT()**  
	Returns Δt(time between frames) in seconds.

* **getFPS()**  
	Returns FPS(1[s] / Δt[s]).
