--!strict
local GameLogic = {}

local Entities = require("./entities")

local PLAYER_LOSS_PAUSE_TICKS = 100

local invaderRerackCallback = Callbacks.register("invasion", "Entities.Invaders.rerack")
local invaderShootCallback = Callbacks.register("invasion", "Entities.Invaders.shoot")
local invaderTouchdownCallback = Callbacks.register("invasion", "GameLogic.onTouchdown")
local resetSceneCallback = Callbacks.register("invasion", "GameLogic.resetScene")
local respawnCallback = Callbacks.register("invasion", "GameLogic.respawnPlayer")

function GameLogic.setScene(): ()
	local hud = Components.HUD()
	hud.remainingLives = 2
	ECS.setHUD(hud)

	local invaderOrchestrationState = Components.InvaderOrchestrationState()
	invaderOrchestrationState.noInvadersCallback = invaderRerackCallback
	invaderOrchestrationState.shootCallback = invaderShootCallback
	invaderOrchestrationState.touchdownCallback = invaderTouchdownCallback
	invaderOrchestrationState.baseTicksPerMove = 4
	invaderOrchestrationState.xSpeed = 8
	invaderOrchestrationState.ySpeed = 12
	invaderOrchestrationState.lastInvaderXSpeed = 16
	invaderOrchestrationState.lastInvaderYSpeed = 12
	invaderOrchestrationState.ticksPerShot = 20
	ECS.setInvaderOrchestrationState(invaderOrchestrationState)

	Entities.Invaders.Eel.scheduleSpawn()

	Entities.Fortress.createAll()

	Entities.Player.new()
end

function GameLogic.resetScene(): ()
	local highScore = ECS.getHUD().highScore

	ECS.clearAll()
	Entities.Invaders.CurrentlyAlive.clearAll()
	Entities.Player.Projectile.resetActiveCount()

	GameLogic.setScene()

	local new_hud = ECS.getHUD()
	new_hud.highScore = highScore
	ECS.setHUD(new_hud)
end

function GameLogic.onTouchdown(): ()
	Game.pause()
	Game.displayGameOver()

	local timeout_entity = ECS.create()
	local cb = Components.CallbackOnTimeoutUnpausable()
	cb.timeoutTicks = PLAYER_LOSS_PAUSE_TICKS
	cb.callback = resetSceneCallback
	ECS.setCallbackOnTimeoutUnpausable(timeout_entity, cb)
end

function GameLogic.onPlayerDefeat(entity: Entity): ()
	Game.pause()
	local position = ECS.getPosition(entity)
	Entities.Player.Explosion.new(position.x, position.y, PLAYER_LOSS_PAUSE_TICKS)

	local timeout_entity = ECS.create()
	local cb = Components.CallbackOnTimeoutUnpausable()
	cb.timeoutTicks = PLAYER_LOSS_PAUSE_TICKS

	local lostAllLives = Game.decrementLives()
	if lostAllLives then
		Game.displayGameOver()
		cb.callback = resetSceneCallback
	else
		cb.callback = respawnCallback
	end

	ECS.setCallbackOnTimeoutUnpausable(timeout_entity, cb)
end

function GameLogic.respawnPlayer(): ()
	Game.unpause()
	Entities.Player.new()
end

return GameLogic
