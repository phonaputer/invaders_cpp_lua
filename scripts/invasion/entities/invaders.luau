--!strict
local Invaders = {}

Invaders.Eel = require("./invader_eel")
Invaders.Octopus = require("./invader_octopus")
Invaders.Jellyfish = require("./invader_jellyfish")
Invaders.Crab = require("./invader_crab")
Invaders.Tadpole = require("./invader_tadpole")
Invaders.Projectile = require("./invader_projectile")
Invaders.CurrentlyAlive = require("./invaders_currently_alive")

function Invaders.rerack(): ()
	local STARTING_X = 2
	local curX = STARTING_X
	local curY = 32
	local ALIEN_WIDTH = 14
	local ALIEN_HEIGHT = 14
	local COL_SPACING = 2
	local ROW_SPACING = -1.2

	local rowInvaderCreationFuncs = {
		Invaders.Octopus.new,
		Invaders.Jellyfish.new,
		Invaders.Crab.new,
		Invaders.Tadpole.new,
		Invaders.Tadpole.new,
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

function Invaders.shoot(): ()
	if math.random(3) ~= 1 then
		return
	end

	local invader, ok = Invaders.CurrentlyAlive.getRandom()
	if not ok then
		return
	end

	local position = ECS.getPosition(invader)
	local xMidpoint = position.x + (position.w / 2)
	local xJitter = math.random(7) - 4
	Invaders.Projectile.new(xMidpoint + xJitter, position.y)
end

return Invaders
