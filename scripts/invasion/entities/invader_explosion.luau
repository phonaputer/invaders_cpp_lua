local InvaderExplosion = {}

local explosionAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(explosionAnimationStrip, 3, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 4, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 5, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 6, 1)

function InvaderExplosion.new(x: number, y: number)
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

return InvaderExplosion
