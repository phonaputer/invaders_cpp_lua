--!strict
local Invasion = {}

Invasion.Entities = require("@self/entities")
Invasion.GameLogic = require("@self/game_logic")

function Invasion.main(): ()
	Invasion.GameLogic.setScene()
end

return Invasion
