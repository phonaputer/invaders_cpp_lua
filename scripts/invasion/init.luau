--!strict
--!nolint FunctionUnused

local Entities = require("@self/entities")

function setScene(): ()
	Entities.Invaders.newJellyfish()

	Entities.Player.new()
end
