--!strict
local PlayerMuzzleFlash = {}

local muzzleFlashStrip = AnimationStrips.create()
AnimationStrips.addFrame(muzzleFlashStrip, 3, 2)
AnimationStrips.addFrame(muzzleFlashStrip, 4, 2)

function PlayerMuzzleFlash.new(player_entity: number): ()
	local entity = ECS.create()

	ECS.setAnimation(entity, {
		ticksPerFrame = 10,
		tickCounter = 0,
		stripID = muzzleFlashStrip,
		curFrame = 0,
		playing = true,
		playReversed = false,
	})

	ECS.setPosition(entity, {
		x = 0,
		y = 0,
		w = 16,
		h = 16,
		z = 110,
	})

	ECS.setPositionFollowing(entity, {
		leader = player_entity,
		xOffset = 0,
		yOffset = 0,
	})

	ECS.setSprite(entity, {
		srcID = "space_invaders.png",
		srcX = 48,
		srcY = 32,
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

return PlayerMuzzleFlash
