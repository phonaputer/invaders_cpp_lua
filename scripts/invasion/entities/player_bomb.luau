--!strict
local PlayerBomb = {}

function PlayerBomb.onProjectileDelete(entity: Entity): ()
	local position = ECS.getPosition(entity)
	PlayerBomb.new(position.x, position.y)
end

local projectileOnDeleteCallback = Callbacks.register("invasion", "Entities.Player.Bomb.onProjectileDelete")

function PlayerBomb.newProjectile(x: number, y: number): ()
	local entity = ECS.create()

	ECS.setCollisionActive(entity, {
		hitboxOffsetX = 7,
		hitboxOffsetY = 6,
		hitboxW = 1,
		hitboxH = 4,
	})

	ECS.setDamage(entity, {
		type = DamageType.PLAYER_PROJECTILE,
		amount = 1,
	})

	ECS.setDeletionCallback(entity, {
		callback = projectileOnDeleteCallback,
	})

	ECS.setHitpoints(entity, {
		susceptibleTo = bit32.bor(DamageType.ALIEN_PROJECTILE, DamageType.ALIEN, DamageType.FORTRESS),
		curHitpoints = 1,
	})

	ECS.setPosition(entity, {
		x = x - 2,
		y = y - 2,
		w = 16,
		h = 16,
		z = 100,
	})

	ECS.setSprite(entity, {
		srcID = "space_invaders.png",
		srcX = 112,
		srcY = 16,
		srcW = 16,
		srcH = 16,
		dstW = 16,
		dstH = 16,
	})

	ECS.setVelocity(entity, {
		x = 0,
		y = -4,
	})
end

local explosionAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(explosionAnimationStrip, 3, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 4, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 5, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 6, 1)

function PlayerBomb.new(x: number, y: number)
	local entity = ECS.create()

	ECS.setAnimation(entity, {
		ticksPerFrame = 6,
		tickCounter = 0,
		stripID = explosionAnimationStrip,
		curFrame = 0,
		playing = true,
		playReversed = false,
	})

	ECS.setPosition(entity, {
		x = x - 16,
		y = y - 16,
		w = 48,
		h = 48,
		z = 200,
	})

	ECS.setSprite(entity, {
		srcID = "space_invaders.png",
		srcX = 48,
		srcY = 16,
		srcW = 16,
		srcH = 16,
		dstW = 48,
		dstH = 48,
	})

	ECS.setTTL(entity, {
		ticksToLive = 24,
		tickCounter = 0,
	})

	local damageEntity = ECS.create()

	ECS.setCollisionActive(damageEntity, {
		hitboxOffsetX = 12,
		hitboxOffsetY = 12,
		hitboxW = 24,
		hitboxH = 22,
	})

	ECS.setDamage(damageEntity, {
		type = DamageType.PLAYER_PROJECTILE,
		amount = 1,
	})

	ECS.setPosition(damageEntity, {
		x = x - 16,
		y = y - 16,
		w = 48,
		h = 48,
		z = 200,
	})

	ECS.setTTL(damageEntity, {
		ticksToLive = 1,
		tickCounter = 0,
	})
end

return PlayerBomb
