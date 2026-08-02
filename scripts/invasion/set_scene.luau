--!strict
--!nolint FunctionUnused

local test = require("./test")
local test2 = require("./test_two")

print(test.add(2, 3))
print(test2.add(5, 6))

function setScene(): ()
	local entity = ECS.create()

	local position = Components.Position()
	position.x = 25
	position.y = 25
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

	local strip = AnimationStrips.create()
	AnimationStrips.addFrame(strip, 1, 0)
	AnimationStrips.addFrame(strip, 0, 0)
	AnimationStrips.addFrame(strip, 1, 0)
	AnimationStrips.addFrame(strip, 2, 0)
	local animation = Components.Animation()
	animation.ticksPerFrame = 25
	animation.stripID = strip

	ECS.setAnimation(entity, animation)
end
