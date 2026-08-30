--!strict
local InvaderEel = {}

local InvaderExplosion = require("./invader_explosion")
local Player = require("./player")

local spawnEelCallback = Callbacks.register("invasion", "Entities.Invaders.Eel.spawn")

function InvaderEel.scheduleSpawn(): ()
	local entity = ECS.create()

	ECS.setCallbackOnTimeout(entity, {
		callback = spawnEelCallback,
		timeoutTicks = math.random(900, 1600),
		tickCounter = 0,
	})
end

function InvaderEel.spawn(): ()
	InvaderEel.new()
	InvaderEel.scheduleSpawn()
end

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

local eelOnDamageCallback = Callbacks.register("invasion", "Entities.Invaders.Eel.onDamage")

function InvaderEel.onDamage(entity: Entity): ()
	local position = ECS.getPosition(entity)
	InvaderExplosion.new(position.x, position.y)
	Game.incrementScore(math.random(100, 300))
	Game.playSound("alien_explosion.wav")
	Player.setRandomBonusWeapon()
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

	ECS.setAnimation(entity, {
		ticksPerFrame = 25,
		tickCounter = 0,
		stripID = strip,
		curFrame = 0,
		playing = true,
		playReversed = false,
	})

	ECS.setCollisionPassive(entity, {
		hitboxOffsetX = 0,
		hitboxOffsetY = 7,
		hitboxW = 24,
		hitboxH = 2,
	})

	ECS.setDamage(entity, {
		type = DamageType.ALIEN,
		amount = 1,
	})

	ECS.setDamageCallback(entity, {
		callback = eelOnDamageCallback,
	})

	ECS.setHitpoints(entity, {
		susceptibleTo = DamageType.PLAYER_PROJECTILE,
		curHitpoints = 1,
	})

	ECS.setPosition(entity, {
		x = startX,
		y = 16,
		w = 24,
		h = 15,
		z = 90,
	})

	ECS.setSprite(entity, {
		srcID = "space_invaders.png",
		srcX = 24 * firstFrame.x,
		srcY = 16 * firstFrame.y,
		srcW = 24,
		srcH = 16,
		dstW = 24,
		dstH = 16,
	})

	ECS.setVelocity(entity, {
		x = xVelocity,
		y = 0,
	})
end

return InvaderEel
