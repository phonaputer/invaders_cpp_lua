--!strict

local Player = {}

local PLAYER_DEATH_PAUSE_TICKS = 100

local onDeleteCallback = Callbacks.register("invasion", "Entities.Player.onDelete")

function Player.onDelete(entity: Entity): ()
	local position = ECS.getPosition(entity)
	newExplosion(position.x, position.y)
end

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
	playerAttack.callback = Callbacks.register("invasion", "Entities.Player.attack")
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

function Player.attack(player_entity: number, x: number, y: number): ()
	if Player.canFire() then
		Player.newProjectile(x, y)
		Player.newMuzzleFlash(player_entity)
	end
end

Player.ProjectileCount = 0

function Player.decrementProjectileCount()
	Player.ProjectileCount -= 1
end

function Player.canFire(): boolean
	return Player.ProjectileCount < 3
end

local projectileOnDeleteCallback = Callbacks.register("invasion", "Entities.Player.decrementProjectileCount")

function Player.newProjectile(x: number, y: number): ()
	Player.ProjectileCount += 1

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

local muzzleFlashStrip = AnimationStrips.create()
AnimationStrips.addFrame(muzzleFlashStrip, 3, 2)
AnimationStrips.addFrame(muzzleFlashStrip, 4, 2)

function Player.newMuzzleFlash(player_entity: number): ()
	local entity = ECS.create()

	local animation = Components.Animation()
	animation.ticksPerFrame = 10
	animation.stripID = muzzleFlashStrip
	ECS.setAnimation(entity, animation)

	local position = Components.Position()
	position.w = 16
	position.h = 16
	position.z = 110
	ECS.setPosition(entity, position)

	local positionFollowing = Components.PositionFollowing()
	positionFollowing.leader = player_entity
	positionFollowing.xOffset = 0
	positionFollowing.yOffset = 0
	ECS.setPositionFollowing(entity, positionFollowing)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = 48
	sprite.srcY = 32
	sprite.srcW = 16
	sprite.srcH = 16
	sprite.dstW = 16
	sprite.dstH = 16
	ECS.setSprite(entity, sprite)

	local ttl = Components.TTL()
	ttl.ticksToLive = 20
	ECS.setTTL(entity, ttl)
end

local explosionAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(explosionAnimationStrip, 1, 3)
AnimationStrips.addFrame(explosionAnimationStrip, 0, 3)
AnimationStrips.addFrame(explosionAnimationStrip, 1, 3)
AnimationStrips.addFrame(explosionAnimationStrip, 2, 3)

function newExplosion(x: number, y: number)
	local entity = ECS.create()

	local animation = Components.Animation()
	animation.ticksPerFrame = 5
	animation.stripID = explosionAnimationStrip
	ECS.setAnimation(entity, animation)

	local position = Components.Position()
	position.x = x
	position.y = y
	position.w = 16
	position.h = 16
	position.z = 100
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
	ttl.ticksToLive = PLAYER_DEATH_PAUSE_TICKS
	ECS.setTTL(entity, ttl)
end

return Player
