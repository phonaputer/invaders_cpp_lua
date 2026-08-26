--!strict
local PlayerExplosion = {}

local explosionAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(explosionAnimationStrip, 1, 3)
AnimationStrips.addFrame(explosionAnimationStrip, 0, 3)
AnimationStrips.addFrame(explosionAnimationStrip, 1, 3)
AnimationStrips.addFrame(explosionAnimationStrip, 2, 3)

function PlayerExplosion.new(x: number, y: number, ticksToLive: number)
	local entity = ECS.create()

	ECS.setAnimationUnpausable(entity, {
		ticksPerFrame = 5,
		tickCounter = 0,
		stripID = explosionAnimationStrip,
		curFrame = 0,
	})

	ECS.setPosition(entity, {
		x = x,
		y = y,
		w = 16,
		h = 16,
		z = 100,
	})

	ECS.setSprite(entity, {
		srcID = "space_invaders.png",
		srcX = 48,
		srcY = 16,
		srcW = 16,
		srcH = 16,
		dstW = 16,
		dstH = 16,
	})

	ECS.setTTL(entity, {
		ticksToLive = ticksToLive,
		tickCounter = 0,
	})
end

return PlayerExplosion
