--!strict

local Invasion = {}

local Entities = require("@self/entities")

function Invasion.setScene(): ()
	Entities.Invaders.newJellyfish(25, 25)

	Entities.Player.new()
end

Invasion.Entities = Entities

return Invasion
