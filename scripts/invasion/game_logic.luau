--!strict
local GameLogic = {}

local Entities = require("./entities")

local invaderRerackCallback = Callbacks.register("invasion", "Entities.Invaders.rerack")
local invaderShootCallback = Callbacks.register("invasion", "Entities.Invaders.shoot")
local invaderTouchdownCallback = Callbacks.register("invasion", "GameLogic.onTouchdown")

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
	print("TOUUUUUCCCHHHHHDDOOOOOWWWNN!!!!!")
end

local PLAYER_DEATH_PAUSE_TICKS = 100

local respawnCallback = Callbacks.register("invasion", "GameLogic.respawnPlayer")
local resetSceneCallback = Callbacks.register("invasion", "GameLogic.resetScene")

function GameLogic.onPlayerDefeat(entity: Entity): ()
	Game.pause()
	local position = ECS.getPosition(entity)
	Entities.Player.Explosion.new(position.x, position.y, PLAYER_DEATH_PAUSE_TICKS)

	local respawn_timeout_entity = ECS.create()
	local cb_on_timeout = Components.CallbackOnTimeoutUnpausable()
	cb_on_timeout.timeoutTicks = PLAYER_DEATH_PAUSE_TICKS

	local lostAllLives = Game.decrementLives()
	if lostAllLives then
		Game.displayGameOver()
		cb_on_timeout.callback = resetSceneCallback
	else
		cb_on_timeout.callback = respawnCallback
	end

	ECS.setCallbackOnTimeoutUnpausable(respawn_timeout_entity, cb_on_timeout)
end

function GameLogic.respawnPlayer(): ()
	Game.unpause()
	Entities.Player.new()
end

return GameLogic
