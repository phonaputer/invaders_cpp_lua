--!strict
local Player = {}

Player.Bomb = require("./player_bomb")
Player.Explosion = require("./player_explosion")
Player.Projectile = require("./player_projectile")
Player.Weapons = require("./player_weapons")

local onDeleteCallback = Callbacks.register("invasion", "GameLogic.onPlayerDefeat")

local playerStrip = AnimationStrips.create()
AnimationStrips.addFrame(playerStrip, 0, 2)
AnimationStrips.addFrame(playerStrip, 1, 2)
AnimationStrips.addFrame(playerStrip, 2, 2)

function Player.new(): ()
	local entity = ECS.create()

	Player.Entity = entity

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
		tickCounter = 0,
		weapon = Player.Weapons.Default,
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

function Player.setDefaultWeapon()
	ECS.setPlayerAttack(Player.Entity, {
		tickCounter = 0,
		weapon = Player.Weapons.Default,
	})
end

Player.SetDefaultWeaponCallback = Callbacks.register("invasion", "Entities.Player.setDefaultWeapon")

return Player
