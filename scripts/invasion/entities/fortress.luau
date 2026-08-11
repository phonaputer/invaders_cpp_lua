--!strict
local Fortress = {}

local STARTING_SRC_X = 48
local STARTING_SRC_Y = 48

Fortress.Tile = require("./fortress_tile")

local function new(x: number, y: number): ()
	for column = 0, 5 do
		local curX = x + column * Fortress.Tile.DRAW_WIDTH
		local curSrcX = STARTING_SRC_X + column * Fortress.Tile.SRC_WIDTH

		for row = 0, 3 do
			if row == 3 and (column == 2 or column == 3) then
				continue
			end

			local curY = y + row * Fortress.Tile.DRAW_HEIGHT
			local curSrcY = STARTING_SRC_Y + row * Fortress.Tile.SRC_HEIGHT

			Fortress.Tile.new({
				x = curX,
				y = curY,
				spriteSrcX = curSrcX,
				spriteSrcY = curSrcY,
			})
		end
	end
end

function Fortress.createAll(): ()
	new(29, 235)
	new(72, 235)
	new(119, 235)
	new(166, 235)
end

return Fortress
