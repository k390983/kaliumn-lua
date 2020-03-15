k = require "kaliumn"

k.initialize("testapp")

width, height = 25, 25

k.initCanvas(width + 1, height + 1, "white")

for i = 0, height do
	for j = 0, width do
		if i == 0 or i == height or j == 0 or j == width then
			k.drawPixel(j, i, "red")

		else
			k.drawPixel(j, i, "blue")

		end

	end

end

k.drawPixel(0, 0, "yellow")
k.drawPixel(width, 0, "yellow")
k.drawPixel(0, height, "yellow")
k.drawPixel(width, height, "yellow")

k.display()

k.waitForKeyPress()

k.terminate()