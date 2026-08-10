--!strict
local Invasion = {}

Invasion.Entities = require("@self/entities")

function Invasion.setScene(): ()
	local hud = Components.HUD()
	hud.remainingLives = 2
	ECS.setHUD(hud)

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

	Invasion.Entities.Invaders.Eel.scheduleSpawn()

	Invasion.Entities.Player.new()
end

function Invasion.resetScene(): ()
	local old_hud = ECS.getHUD()
	local highScore = old_hud.highScore

	ECS.clearAll()
	Invasion.Entities.Invaders.CurrentlyAlive.clearAll()
	Invasion.Entities.Player.Projectile.resetActiveCount()
	Invasion.setScene()

	local new_hud = ECS.getHUD()
	new_hud.highScore = highScore
	ECS.setHUD(new_hud)
end

return Invasion
