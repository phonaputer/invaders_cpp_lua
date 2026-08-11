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

	local animation = Components.Animation()
	animation.ticksPerFrame = 4
	animation.stripID = playerStrip
	animation.playing = false
	ECS.setAnimation(entity, animation)

	local collision = Components.CollisionPassive()
	collision.hitboxOffsetX = 0
	collision.hitboxOffsetY = 7
	collision.hitboxW = 15
	collision.hitboxH = 5
	ECS.setCollisionPassive(entity, collision)

	local damage = Components.Damage()
	damage.type = DamageType.PLAYER
	damage.amount = 1
	ECS.setDamage(entity, damage)

	local deletionCallback = Components.DeletionCallback()
	deletionCallback.callback = onDeleteCallback
	ECS.setDeletionCallback(entity, deletionCallback)

	local hitpoints = Components.Hitpoints()
	hitpoints.susceptibleTo = DamageType.ALIEN_PROJECTILE
	hitpoints.curHitpoints = 1
	ECS.setHitpoints(entity, hitpoints)

	local playerAttack = Components.PlayerAttack()
	playerAttack.ticksPerAttack = 8
	playerAttack.callback = onAttackCallback
	ECS.setPlayerAttack(entity, playerAttack)

	local playerMovement = Components.PlayerMovement()
	playerMovement.xSpeed = 1
	ECS.setPlayerMovement(entity, playerMovement)

	local position = Components.Position()
	position.x = Game.WINDOW_WIDTH / 2 - 8
	position.y = Game.WINDOW_HEIGHT - 35
	position.w = 16
	position.h = 16
	position.z = 100
	ECS.setPosition(entity, position)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = 0
	sprite.srcY = 32
	sprite.srcW = 16
	sprite.srcH = 16
	sprite.dstW = 16
	sprite.dstH = 16
	ECS.setSprite(entity, sprite)
end

return Player
