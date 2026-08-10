--!strict
local Invasion = {}

Invasion.Entities = require("@self/entities")
Invasion.Game = require("@self/game")

function Invasion.main(): ()
	Invasion.Game.setScene()
end

return Invasion
