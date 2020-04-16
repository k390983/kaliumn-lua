kal = require "kaliumn"

--============================================================================--
-- 1. Window Dimensions / Canvas Size
--============================================================================--

kal.init("Test")

width, height = kal.getWinWidth(), kal.getWinHeight()

kal.initCanvas(width, height, 0, 0, 255)

kal.drawTexture(20, 20, "assets/test.png")
kal.drawPixel(10, 10, 255, 0, 0, 100)

kal.displayCanvas()

kal.waitForKeyPress()

kal.term()
