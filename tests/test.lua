function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. ": "
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    else
      print(formatting .. v)
    end
  end
end

kal = require "kaliumn"

--[[kal.init("Test")

width, height = kal.getWinWidth(), kal.getWinHeight()

kal.initCanvas(width, height, 0, 0, 255)

kal.drawTexture(20, 20, "assets/test.png")
kal.drawPixel(10, 10, 255, 0, 0, 100)

kal.displayCanvas()

kal.waitForKeyPress()

kal.term()]]

width, height = kal.getWinWidth(), kal.getWinHeight()

kal.initCanvas(width, height, 0, 0, 255)

texture = kal.initTexture("assets/test.png")

kal.drawTexture(texture, 10, 10)

kal.drawPixel(255, 0, 0, 255, 20, 20)

kal.displayCanvas()

kal.waitForKeyPress()

kal.term()
