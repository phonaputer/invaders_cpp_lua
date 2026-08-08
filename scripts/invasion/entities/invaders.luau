--!strict

local Invaders = {}

function Invaders.onDeleteExplode(entity: Entity): ()
	local position = ECS.getPosition(entity)
	Invaders.newExplosion(position.x, position.y)
end

function Invaders.newJellyfish(x: number, y: number)
	local entity = ECS.create()

	local strip = AnimationStrips.create()
	AnimationStrips.addFrame(strip, 1, 0)
	AnimationStrips.addFrame(strip, 0, 0)
	AnimationStrips.addFrame(strip, 1, 0)
	AnimationStrips.addFrame(strip, 2, 0)
	local animation = Components.Animation()
	animation.ticksPerFrame = 25
	animation.stripID = strip
	ECS.setAnimation(entity, animation)

	local collision = Components.CollisionPassive()
	collision.hitboxOffsetX = 1
	collision.hitboxOffsetY = 3
	collision.hitboxW = 12
	collision.hitboxH = 8
	ECS.setCollisionPassive(entity, collision)

	local damage = Components.Damage()
	damage.type = DamageType.ALIEN
	damage.amount = 1
	ECS.setDamage(entity, damage)

	local deletionCallback = Components.DeletionCallback()
	deletionCallback.callback = Callbacks.register("invasion", "Entities.Invaders.onDeleteExplode")
	ECS.setDeletionCallback(entity, deletionCallback)

	local hitpoints = Components.Hitpoints()
	hitpoints.susceptibleTo = DamageType.PLAYER_PROJECTILE
	hitpoints.curHitpoints = 1
	ECS.setHitpoints(entity, hitpoints)

	local position = Components.Position()
	position.x = x
	position.y = y
	position.w = 16
	position.h = 16
	position.z = 1
	ECS.setPosition(entity, position)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = 16
	sprite.srcY = 0
	sprite.srcW = 16
	sprite.srcH = 16
	sprite.dstW = 16
	sprite.dstH = 16
	ECS.setSprite(entity, sprite)
end

function Invaders.newExplosion(x: number, y: number)
	local entity = ECS.create()

	local strip = AnimationStrips.create()
	AnimationStrips.addFrame(strip, 3, 1)
	AnimationStrips.addFrame(strip, 4, 1)
	AnimationStrips.addFrame(strip, 5, 1)
	AnimationStrips.addFrame(strip, 6, 1)
	local animation = Components.Animation()
	animation.ticksPerFrame = 5
	animation.stripID = strip
	ECS.setAnimation(entity, animation)

	local position = Components.Position()
	position.x = x
	position.y = y
	position.w = 16
	position.h = 16
	position.z = 200
	ECS.setPosition(entity, position)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = 48
	sprite.srcY = 16
	sprite.srcW = 16
	sprite.srcH = 16
	sprite.dstW = 16
	sprite.dstH = 16
	ECS.setSprite(entity, sprite)

	local ttl = Components.TTL()
	ttl.ticksToLive = 20
	ECS.setTTL(entity, ttl)
end

return Invaders
