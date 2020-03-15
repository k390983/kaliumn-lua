k = require "kaliumn"

k.initialize()

k.setCursorPosition(10, 10)

k.setColor("red", "blue")
k.printString("width: "..k.getWinX()..", height: "..k.getWinY().."\n\n")
k.setColor("cyan", "magenta")

k.terminate()