--!strict
local PlayerMuzzleFlash = {}

local muzzleFlashStrip = AnimationStrips.create()
AnimationStrips.addFrame(muzzleFlashStrip, 3, 2)
AnimationStrips.addFrame(muzzleFlashStrip, 4, 2)

function PlayerMuzzleFlash.new(player_entity: number): ()
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

return PlayerMuzzleFlash
