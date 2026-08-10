--!strict
local PlayerProjectile = {}

local projectileCount = 0

function PlayerProjectile.decrementCount()
	projectileCount -= 1
end

function PlayerProjectile.getCount(): number
	return projectileCount
end

local projectileOnDeleteCallback = Callbacks.register("invasion", "Entities.Player.Projectile.decrementCount")

function PlayerProjectile.new(x: number, y: number): ()
	projectileCount += 1

	local entity = ECS.create()

	local collision = Components.CollisionActive()
	collision.hitboxOffsetX = 7
	collision.hitboxOffsetY = 6
	collision.hitboxW = 1
	collision.hitboxH = 4
	ECS.setCollisionActive(entity, collision)

	local damage = Components.Damage()
	damage.type = DamageType.PLAYER_PROJECTILE
	damage.amount = 1
	ECS.setDamage(entity, damage)

	local deletionCallback = Components.DeletionCallback()
	deletionCallback.callback = projectileOnDeleteCallback
	ECS.setDeletionCallback(entity, deletionCallback)

	local hitpoints = Components.Hitpoints()
	hitpoints.susceptibleTo = bit32.bor(DamageType.ALIEN_PROJECTILE, DamageType.ALIEN, DamageType.FORTRESS)
	hitpoints.curHitpoints = 1
	ECS.setHitpoints(entity, hitpoints)

	local position = Components.Position()
	position.x = x - 2
	position.y = y - 2
	position.w = 16
	position.h = 16
	position.z = 100
	ECS.setPosition(entity, position)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = 112
	sprite.srcY = 16
	sprite.srcW = 16
	sprite.srcH = 16
	sprite.dstW = 16
	sprite.dstH = 16
	ECS.setSprite(entity, sprite)

	local velocity = Components.Velocity()
	velocity.y = -4
	ECS.setVelocity(entity, velocity)
end

return PlayerProjectile
