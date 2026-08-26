--!strict
local GameLogic = {}

local Entities = require("./entities")

local PLAYER_LOSS_PAUSE_TICKS = 150

local invaderRerackCallback = Callbacks.register("invasion", "Entities.Invaders.rerack")
local invaderShootCallback = Callbacks.register("invasion", "Entities.Invaders.shoot")
local invaderTouchdownCallback = Callbacks.register("invasion", "GameLogic.onTouchdown")
local resetSceneCallback = Callbacks.register("invasion", "GameLogic.resetScene")
local respawnCallback = Callbacks.register("invasion", "GameLogic.respawnPlayer")

function GameLogic.setScene(): ()
	ECS.setHUD({
		score = 0,
		highScore = 0,
		remainingLives = 2,
	})

	ECS.setInvaderOrchestrationState({
		noInvadersCallback = invaderRerackCallback,
		shootCallback = invaderShootCallback,
		touchdownCallback = invaderTouchdownCallback,
		baseTicksPerMove = 4,
		tickCounter = 0,
		xSpeed = 8,
		ySpeed = 12,
		lastInvaderXSpeed = 16,
		lastInvaderYSpeed = 12,
		ticksPerShot = 20,
		shootTickCounter = 0,
		curArp = 0,
		movingLeft = false,
	})

	Entities.Invaders.Eel.scheduleSpawn()

	Entities.Fortress.createAll()

	Entities.Player.new()
end

function GameLogic.resetScene(): ()
	local highScore = ECS.getHUD().highScore

	ECS.clearAll()
	Entities.Invaders.CurrentlyAlive.clearAll()
	Entities.Player.Projectile.resetActiveCount()
	Game.stopSound("player_explosion.wav")

	GameLogic.setScene()

	local new_hud = ECS.getHUD()
	new_hud.highScore = highScore
	ECS.setHUD(new_hud)
end

function GameLogic.onTouchdown(): ()
	Game.pause()
	Game.displayGameOver()

	ECS.setCallbackOnTimeoutUnpausable(ECS.create(), {
		timeoutTicks = PLAYER_LOSS_PAUSE_TICKS,
		tickCounter = 0,
		callback = resetSceneCallback,
	})
end

function GameLogic.onPlayerDefeat(entity: Entity): ()
	Game.pause()
	local position = ECS.getPosition(entity)
	Entities.Player.Explosion.new(position.x, position.y, PLAYER_LOSS_PAUSE_TICKS)
	Game.playSound("player_explosion.wav")

	local callback = respawnCallback

	local lostAllLives = Game.decrementLives()
	if lostAllLives then
		Game.displayGameOver()
		callback = resetSceneCallback
	end

	ECS.setCallbackOnTimeoutUnpausable(ECS.create(), {
		timeoutTicks = PLAYER_LOSS_PAUSE_TICKS,
		tickCounter = 0,
		callback = callback,
	})
end

function GameLogic.respawnPlayer(): ()
	Game.stopSound("player_explosion.wav")
	Game.unpause()
	Entities.Player.new()
end

return GameLogic
