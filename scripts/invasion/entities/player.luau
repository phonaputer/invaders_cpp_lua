--!strict

local Player = {}

function Player.new(): ()
	local entity = ECS.create()

	local strip = AnimationStrips.create()
	AnimationStrips.addFrame(strip, 0, 2)
	AnimationStrips.addFrame(strip, 1, 2)
	AnimationStrips.addFrame(strip, 2, 2)
	local animation = Components.Animation()
	animation.ticksPerFrame = 4
	animation.stripID = strip
	animation.playing = false
	ECS.setAnimation(entity, animation)

	local collision = Components.Collision()
	collision.hitboxOffsetX = 0
	collision.hitboxOffsetY = 7
	collision.hitboxW = 15
	collision.hitboxH = 5
	ECS.setCollision(entity, collision)

	local playerAttack = Components.PlayerAttack()
	playerAttack.ticksPerAttack = 8
	playerAttack.callback = "Entities.Player.newProjectile"
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

function Player.newProjectile(x: number, y: number): ()
	local entity = ECS.create()

	local collision = Components.Collision()
	collision.hitboxOffsetX = 7
	collision.hitboxOffsetY = 6
	collision.hitboxW = 1
	collision.hitboxH = 4
	ECS.setCollision(entity, collision)

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

return Player
