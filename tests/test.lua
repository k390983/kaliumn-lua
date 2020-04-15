kal = require "kaliumn"

--============================================================================--
-- 1. Window Dimensions / Canvas Size
--============================================================================--

kal.initialize("Test")

width, height = kal.getWinX(), kal.getWinY()

kal.initCanvas(width, height, 0, 0, 255)

--[[for i = 0, height - 1 do
	for j = 0, width - 1 do
		if j == math.floor(width / 2) or  i == math.floor(height / 2) then
			kal.drawPixel(j, i, "green")

		end

		if i == 0 or i == height - 1 or j == 0 or j == width - 1 then
			kal.drawPixel(j, i, "red")

		end

	end

end]]

kal.display()

kal.waitForKeyPress()

kal.terminate()

