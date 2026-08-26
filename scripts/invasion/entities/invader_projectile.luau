--!strict
local InvaderProjectile = {}

local InvaderExplosion = require("./invader_explosion")

local projectileAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(projectileAnimationStrip, 5, 2)
AnimationStrips.addFrame(projectileAnimationStrip, 6, 2)

local projectileOnDamageCallback = Callbacks.register("invasion", "Entities.Invaders.Projectile.onDamage")

function InvaderProjectile.onDamage(entity: Entity): ()
	local position = ECS.getPosition(entity)
	InvaderExplosion.new(position.x, position.y)
	Game.playSound("alien_shot.wav")
end

function InvaderProjectile.new(x: number, y: number): ()
	local entity = ECS.create()

	ECS.setAnimation(entity, {
		ticksPerFrame = 40,
		tickCounter = 0,
		stripID = projectileAnimationStrip,
		curFrame = 0,
		playing = true,
		playReversed = false,
	})

	ECS.setCollisionActive(entity, {
		hitboxOffsetX = 7,
		hitboxOffsetY = 7,
		hitboxW = 1,
		hitboxH = 3,
	})

	ECS.setDamage(entity, {
		type = DamageType.ALIEN_PROJECTILE,
		amount = 1,
	})

	ECS.setDamageCallback(entity, {
		callback = projectileOnDamageCallback,
	})

	ECS.setHitpoints(entity, {
		susceptibleTo = bit32.bor(DamageType.PLAYER, DamageType.PLAYER_PROJECTILE, DamageType.FORTRESS),
		curHitpoints = 1,
	})

	ECS.setPosition(entity, {
		x = x,
		y = y,
		w = 16,
		h = 16,
		z = 80,
	})

	ECS.setSprite(entity, {
		srcID = "space_invaders.png",
		srcX = 80,
		srcY = 32,
		srcW = 16,
		srcH = 16,
		dstW = 16,
		dstH = 16,
	})

	ECS.setVelocity(entity, {
		x = 0,
		y = 1,
	})
end

return InvaderProjectile
