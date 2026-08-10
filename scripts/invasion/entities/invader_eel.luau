local InvaderExplosion = require("./invader_explosion")

local InvaderEel = {}

local eelLeftStrip = AnimationStrips.create()
AnimationStrips.addFrame(eelLeftStrip, 0, 4)
AnimationStrips.addFrame(eelLeftStrip, 1, 4)
AnimationStrips.addFrame(eelLeftStrip, 0, 4)
AnimationStrips.addFrame(eelLeftStrip, 2, 4)

local eelRightStrip = AnimationStrips.create()
AnimationStrips.addFrame(eelRightStrip, 4, 5)
AnimationStrips.addFrame(eelRightStrip, 4, 4)
AnimationStrips.addFrame(eelRightStrip, 4, 5)
AnimationStrips.addFrame(eelRightStrip, 3, 4)

local eelOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.Eel.onDelete")

function InvaderEel.onDelete(entity: Entity): ()
	local position = ECS.getPosition(entity)
	InvaderExplosion.new(position.x, position.y)
	Game.incrementScore(math.random(100, 300))
end

type FirstFrame = {
	x: number,
	y: number,
}

function InvaderEel.new(): ()
	local entity = ECS.create()

	local xVelocity = 0.5
	local startX = -24
	local strip = eelRightStrip
	local firstFrame: FirstFrame = { x = 4, y = 5 }

	if math.random(2) == 1 then
		xVelocity = -xVelocity
		startX = Game.WINDOW_WIDTH
		strip = eelLeftStrip
		firstFrame = { x = 0, y = 4 }
	end

	local animation = Components.Animation()
	animation.ticksPerFrame = 25
	animation.stripID = strip
	animation.playing = true
	ECS.setAnimation(entity, animation)

	local collision = Components.CollisionPassive()
	collision.hitboxOffsetX = 0
	collision.hitboxOffsetY = 7
	collision.hitboxW = 24
	collision.hitboxH = 2
	ECS.setCollisionPassive(entity, collision)

	local damage = Components.Damage()
	damage.type = DamageType.ALIEN
	damage.amount = 1
	ECS.setDamage(entity, damage)

	local deletionCallback = Components.DeletionCallback()
	deletionCallback.callback = eelOnDeleteCallback
	ECS.setDeletionCallback(entity, deletionCallback)

	local hitpoints = Components.Hitpoints()
	hitpoints.susceptibleTo = DamageType.PLAYER_PROJECTILE
	hitpoints.curHitpoints = 1
	ECS.setHitpoints(entity, hitpoints)

	local position = Components.Position()
	position.x = startX
	position.y = 16
	position.w = 24
	position.h = 15
	position.z = 90
	ECS.setPosition(entity, position)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = 24 * firstFrame.x
	sprite.srcY = 16 * firstFrame.y
	sprite.srcW = 24
	sprite.srcH = 16
	sprite.dstW = 24
	sprite.dstH = 16
	ECS.setSprite(entity, sprite)

	local velocity = Components.Velocity()
	velocity.x = xVelocity
	ECS.setVelocity(entity, velocity)
end

return InvaderEel
