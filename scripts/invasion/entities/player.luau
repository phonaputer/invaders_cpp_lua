--!strict
local Player = {}

Player.Projectile = require("./player_projectile")

Player.Explosion = require("./player_explosion")
local PlayerMuzzleFlash = require("./player_muzzle_flash")

function Player.attack(player_entity: number, x: number, y: number): ()
	if Player.Projectile.getActiveCount() < 3 then
		Player.Projectile.new(x, y)
		PlayerMuzzleFlash.new(player_entity)
		Game.playSound("player_shot.wav")
	end
end

local onDeleteCallback = Callbacks.register("invasion", "GameLogic.onPlayerDefeat")
local onAttackCallback = Callbacks.register("invasion", "Entities.Player.attack")

local playerStrip = AnimationStrips.create()
AnimationStrips.addFrame(playerStrip, 0, 2)
AnimationStrips.addFrame(playerStrip, 1, 2)
AnimationStrips.addFrame(playerStrip, 2, 2)

function Player.new(): ()
	local entity = ECS.create()

	ECS.setAnimation(entity, {
		ticksPerFrame = 4,
		tickCounter = 0,
		stripID = playerStrip,
		curFrame = 0,
		playing = false,
		playReversed = false,
	})

	ECS.setCollisionPassive(entity, {
		hitboxOffsetX = 0,
		hitboxOffsetY = 7,
		hitboxW = 15,
		hitboxH = 5,
	})

	ECS.setDamage(entity, {
		type = DamageType.PLAYER,
		amount = 1,
	})

	ECS.setDeletionCallback(entity, {
		callback = onDeleteCallback,
	})

	ECS.setHitpoints(entity, {
		susceptibleTo = DamageType.ALIEN_PROJECTILE,
		curHitpoints = 1,
	})

	ECS.setPlayerAttack(entity, {
		ticksPerAttack = 8,
		tickCounter = 0,
		callback = onAttackCallback,
	})

	ECS.setPlayerMovement(entity, {
		xSpeed = 1,
	})

	ECS.setPosition(entity, {
		x = Game.WINDOW_WIDTH / 2 - 8,
		y = Game.WINDOW_HEIGHT - 35,
		w = 16,
		h = 16,
		z = 100,
	})

	ECS.setSprite(entity, {
		srcID = "space_invaders.png",
		srcX = 0,
		srcY = 32,
		srcW = 16,
		srcH = 16,
		dstW = 16,
		dstH = 16,
	})
end

return Player
