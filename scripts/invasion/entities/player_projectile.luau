--!strict
local PlayerProjectile = {}

local projectileCount = 0

function PlayerProjectile.resetActiveCount(): ()
	projectileCount = 0
end

function PlayerProjectile.decrementActiveCount(): ()
	projectileCount -= 1
end

function PlayerProjectile.getActiveCount(): number
	return projectileCount
end

local projectileOnDeleteCallback = Callbacks.register("invasion", "Entities.Player.Projectile.decrementActiveCount")

function PlayerProjectile.new(x: number, y: number): ()
	projectileCount += 1

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

return PlayerProjectile
