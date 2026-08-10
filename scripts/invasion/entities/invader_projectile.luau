local InvaderExplosion = require("./invader_explosion")

local InvaderProjectile = {}

local projectileAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(projectileAnimationStrip, 5, 2)
AnimationStrips.addFrame(projectileAnimationStrip, 6, 2)

local projectileOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.Projectile.onDelete")

function InvaderProjectile.onDelete(entity: Entity): ()
	local position = ECS.getPosition(entity)
	InvaderExplosion.new(position.x, position.y)
end

function InvaderProjectile.new(x: number, y: number): ()
	local entity = ECS.create()

	local animation = Components.Animation()
	animation.ticksPerFrame = 40
	animation.stripID = projectileAnimationStrip
	ECS.setAnimation(entity, animation)

	local collision = Components.CollisionActive()
	collision.hitboxOffsetX = 7
	collision.hitboxOffsetY = 7
	collision.hitboxW = 1
	collision.hitboxH = 3
	ECS.setCollisionActive(entity, collision)

	local damage = Components.Damage()
	damage.type = DamageType.ALIEN_PROJECTILE
	damage.amount = 1
	ECS.setDamage(entity, damage)

	local deletionCallback = Components.DeletionCallback()
	deletionCallback.callback = projectileOnDeleteCallback
	ECS.setDeletionCallback(entity, deletionCallback)

	local hitpoints = Components.Hitpoints()
	hitpoints.susceptibleTo = bit32.bor(DamageType.PLAYER, DamageType.PLAYER_PROJECTILE, DamageType.FORTRESS)
	hitpoints.curHitpoints = 1
	ECS.setHitpoints(entity, hitpoints)

	local position = Components.Position()
	position.x = x
	position.y = y
	position.w = 16
	position.h = 16
	position.z = 80
	ECS.setPosition(entity, position)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = 80
	sprite.srcY = 32
	sprite.srcW = 16
	sprite.srcH = 16
	sprite.dstW = 16
	sprite.dstH = 16
	ECS.setSprite(entity, sprite)

	local velocity = Components.Velocity()
	velocity.y = 1
	ECS.setVelocity(entity, velocity)
end

return InvaderProjectile
