k = require "kaliumn"

k.initialize("testapp")

k.moveCursor(10, 10)

k.setColor("red", "blue")
k.printString("width: "..k.getWinX()..", height: "..k.getWinY().."\n\n")

k.waitForKeyPress()

k.terminate()