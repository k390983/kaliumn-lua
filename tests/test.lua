kal = require "kaliumn"

kal.initialize(12)

--============================================================================--
-- 1. Window Dimensions / Canvas Size
--============================================================================--

os.execute("clear")
print("\n ==== Test 1 - Window Dimensions / Canvas Size ====")
kal.waitForKeyPress()

kal.initialize("Test 1 - Window Dimensions / Canvas Size")

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
-- 2. Display All Colors
--============================================================================--

os.execute("clear")
print("\n ==== Test 2 - Display All Colors ====")
kal.waitForKeyPress()

kal.initialize("Test 2 - Display All Colors")

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

--============================================================================--
-- 3. Textures
--============================================================================--

os.execute("clear")
print("\n ==== Test 3 - Textures ====")
kal.waitForKeyPress()

kal.initialize("Test 3 - Textures")

width, height = kal.getWinX(), kal.getWinY()

kal.initCanvas(width, height, "default")

kal.drawTexture(5, 5, "texture.kal")

kal.display()

kal.waitForKeyPress()

kal.terminate()

--============================================================================--
-- 4. Animation
--============================================================================--

os.execute("clear")
print("\n ==== Test 4 - Animation ====")
kal.waitForKeyPress()

kal.initialize("Test 4 - Animation")

width, height = kal.getWinX(), kal.getWinY()

kal.initCanvas(width, height, "default")

x, y, xDirection, yDirection, lastFrame = 0, 0, 0, 0, 0

while true do
	if kal.getTime() - lastFrame > 100 then
		lastFrame = kal.getTime()

		if x == 0 then xDirection = 0 end
		if x + 8 == width then xDirection = 1 end
		if y == 0 then yDirection = 0 end
		if y + 8 == height then yDirection = 1 end
				
		if xDirection == 0 then x = x + 1 else x = x - 1 end
		if yDirection == 0 then y = y + 1 else y = y - 1 end

		kal.cleanCanvas("default")

		kal.drawTexture(x, y, "texture.kal")

		kal.display()

	end

end

kal.waitForKeyPress()

kal.terminate()