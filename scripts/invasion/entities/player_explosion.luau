--!strict
local PlayerExplosion = {}

local explosionAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(explosionAnimationStrip, 1, 3)
AnimationStrips.addFrame(explosionAnimationStrip, 0, 3)
AnimationStrips.addFrame(explosionAnimationStrip, 1, 3)
AnimationStrips.addFrame(explosionAnimationStrip, 2, 3)

function PlayerExplosion.new(x: number, y: number, ticksToLive: number)
	local entity = ECS.create()

	local animation = Components.AnimationUnpausable()
	animation.ticksPerFrame = 5
	animation.stripID = explosionAnimationStrip
	ECS.setAnimationUnpausable(entity, animation)

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
	ttl.ticksToLive = ticksToLive
	ECS.setTTL(entity, ttl)
end

return PlayerExplosion
