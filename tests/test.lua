function printTable(tbl, indent)
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

package.path = package.path .. ";../?.lua"
kal = require "kaliumn"

--[[width, height = kal.getWinX(), kal.getWinY()

kal.initCanvas(width, height, 0, 0, 255)

ball = {
	x = width / 2,
	y = height /2,
	xdir = 1,
	ydir = 1,
	texture = kal.initTexture("assets/ball.png")
}

while true do
	if ball.x < 0 or ball.x > width - 10 then
		ball.xdir = ball.xdir * -1
	end

	if ball.y < 0 or ball.y > height - 10 then
		ball.ydir = ball.ydir * -1
	end

	ball.x = ball.x + ball.xdir
	ball.y = ball.y + ball.ydir

	kal.cleanCanvas(20, 20, 20)
	kal.drawTexture(ball.texture, ball.x, ball.y)
	kal.displayCanvas()
	--kal.waitForKeyPress()

end

--print(kal.getTime())

--kal.waitForKeyPress()

--print(kal.getTime())

--kal.waitForKeyPress()

kal.term()]]

print(kal.getTime())
kal.waitForKeyPress()
print(kal.getTime())
