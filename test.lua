kal = require "kaliumn"

--============================================================================--
-- Window
--============================================================================--

kal.initialize("Window")

width, height = kal.getWinX(), kal.getWinY()

kal.initCanvas(width, height, "blue")

for i = 0, height - 1 do
	for j = 0, width - 1 do
		if j == math.floor(width / 2) or  i == math.floor(height / 2) then
			kal.drawPixel(j, i, "green")

		end

		if i == 0 or i == height - 1 or j == 0 or j == width - 1 then
			kal.drawPixel(j, i, "red")

		end

	end

end

kal.display()

kal.waitForKeyPress()

kal.terminate()

--============================================================================--
-- Display all colors
--============================================================================--

kal.initialize("Display all colors")

width, height = kal.getWinX(), kal.getWinY()

kal.initCanvas(width, height, "default")

colors = {
	"black",
	"red",
	"green",
	"yellow",
	"blue",
	"magenta",
	"cyan",
	"light gray",
	"dark gray",
	"light red",
	"light green",
	"light yellow",
	"light blue",
	"light magenta",
	"light cyan",
	"white",
	"default"

}

for i = 0, 16 do
	for j = 0, width do
		kal.drawPixel(j, i, colors[i + 1])

	end

end

kal.display()

kal.waitForKeyPress()

kal.terminate()