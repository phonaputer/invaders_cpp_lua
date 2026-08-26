--!strict
local InvaderExplosion = {}

local explosionAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(explosionAnimationStrip, 3, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 4, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 5, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 6, 1)

function InvaderExplosion.new(x: number, y: number)
	local entity = ECS.create()

	ECS.setAnimation(entity, {
		ticksPerFrame = 5,
		tickCounter = 0,
		stripID = explosionAnimationStrip,
		curFrame = 0,
		playing = true,
		playReversed = false,
	})

	ECS.setPosition(entity, {
		x = x,
		y = y,
		w = 16,
		h = 16,
		z = 200,
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
		ticksToLive = 20,
		tickCounter = 0,
	})
end

return InvaderExplosion
