--!strict

local Invasion = {}

Invasion.Entities = require("@self/entities")

local _thang = require("@self/testone")
local testTwo = require("@self/testtwo")

function Invasion.setScene(): ()
	testTwo.IncrExample()

	local invaderOrchestrationState = Components.InvaderOrchestrationState()
	invaderOrchestrationState.noInvadersCallback = Callbacks.register("invasion", "Entities.Invaders.rerack")
	invaderOrchestrationState.shootCallback = Callbacks.register("invasion", "Entities.Invaders.shoot")
	invaderOrchestrationState.baseTicksPerMove = 4
	invaderOrchestrationState.xSpeed = 8
	invaderOrchestrationState.ySpeed = 12
	invaderOrchestrationState.lastInvaderXSpeed = 16
	invaderOrchestrationState.lastInvaderYSpeed = 12
	invaderOrchestrationState.ticksPerShot = 20
	ECS.setInvaderOrchestrationState(invaderOrchestrationState)

	Invasion.Entities.Player.new()
end

return Invasion
