k = require "kaliumn"

k.initialize("testapp")

k.moveCursor(10, 10)

k.setColor("red", "blue")
k.printString("width: "..k.getWinX()..", height: "..k.getWinY().."\n\n")

k.waitForKeyPress()

k.initCanvas(25, 25, "white")

k.drawPixel(5, 5, "blue")

k.display()

k.waitForKeyPress()

k.terminate()