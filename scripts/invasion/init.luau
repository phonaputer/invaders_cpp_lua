--!strict

local Invasion = {}

Invasion.Entities = require("@self/entities")

function Invasion.setScene(): ()
	local invaderOrchestrationState = Components.InvaderOrchestrationState()
	invaderOrchestrationState.noInvadersCallback = Callbacks.register("invasion", "Entities.Invaders.rerack")
	invaderOrchestrationState.shootCallback = Callbacks.register("invasion", "Entities.Invaders.shoot")
	invaderOrchestrationState.baseTicksPerMove = 4
	invaderOrchestrationState.xSpeed = 8
	invaderOrchestrationState.ySpeed = 12
	invaderOrchestrationState.lastInvaderXSpeed = 16
	invaderOrchestrationState.lastInvaderYSpeed = 12
	invaderOrchestrationState.ticksPerShot = 50
	ECS.setInvaderOrchestrationState(invaderOrchestrationState)

	Invasion.Entities.Player.new()
end

return Invasion
