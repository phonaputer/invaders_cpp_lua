--!strict

local Invasion = {}

Invasion.Entities = require("@self/entities")

function Invasion.setScene(): ()
	Invasion.Entities.Invaders.newJellyfish(25, 25)

	Invasion.Entities.Player.new()
end

return Invasion
