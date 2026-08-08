--!strict

local Invasion = {}

Invasion.Entities = require("@self/entities")

function Invasion.setScene(): ()
	Invasion.rackInvaders()

	Invasion.Entities.Player.new()
end

function Invasion.rackInvaders(): ()
	local STARTING_X = 2
	local curX = STARTING_X
	local curY = 32
	local ALIEN_WIDTH = 14
	local ALIEN_HEIGHT = 14
	local COL_SPACING = 2
	local ROW_SPACING = -1.2

	local rowInvaderCreationFuncs = {
		Invasion.Entities.Invaders.newOctopus,
		Invasion.Entities.Invaders.newJellyfish,
		Invasion.Entities.Invaders.newCrab,
		Invasion.Entities.Invaders.newTadpole,
		Invasion.Entities.Invaders.newTadpole,
	}

	for _, createInvader in rowInvaderCreationFuncs do
		for i = 1, 11 do
			createInvader(curX, curY)
			curX += ALIEN_WIDTH + COL_SPACING
		end

		curX = STARTING_X
		curY += ALIEN_HEIGHT + ROW_SPACING
	end
end

return Invasion
