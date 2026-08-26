--!strict
local BaseInvader = {}

local InvaderExplosion = require("./invader_explosion")
local InvadersCurrentlyAlive = require("./invaders_currently_alive")

export type NewInvaderArgs = {
	x: number,
	y: number,
	animationStrip: AnimationStrip,
	animationFirstFrame: Frame,
	onDeleteCallback: number,
	hitboxOffsetX: number,
	hitboxOffsetY: number,
	hitboxW: number,
	hitboxH: number,
}

export type Frame = {
	x: number,
	y: number,
}

function BaseInvader.onDelete(entity: Entity): ()
	local position = ECS.getPosition(entity)
	InvaderExplosion.new(position.x, position.y)
	InvadersCurrentlyAlive.remove(entity)
	Game.playSound("alien_explosion.wav")
end

function BaseInvader.new(args: NewInvaderArgs)
	local entity = ECS.create()

	InvadersCurrentlyAlive.add(entity)

	ECS.setCollisionPassive(entity, {
		hitboxOffsetX = args.hitboxOffsetX,
		hitboxOffsetY = args.hitboxOffsetY,
		hitboxW = args.hitboxW,
		hitboxH = args.hitboxH,
	})

	ECS.setDamage(entity, {
		type = DamageType.ALIEN,
		amount = 1,
	})

	ECS.setDeletionCallback(entity, {
		callback = args.onDeleteCallback,
	})

	ECS.setHitpoints(entity, {
		susceptibleTo = DamageType.PLAYER_PROJECTILE,
		curHitpoints = 1,
	})

	ECS.setInvaderAnimation(entity, {
		stripID = args.animationStrip,
		curFrame = 0,
	})

	ECS.setOrchestratedInvader(entity)

	ECS.setPosition(entity, {
		x = args.x,
		y = args.y,
		w = 16,
		h = 16,
		z = 1,
	})

	ECS.setSprite(entity, {
		srcID = "space_invaders.png",
		srcX = 16 * args.animationFirstFrame.x,
		srcY = 16 * args.animationFirstFrame.y,
		srcW = 16,
		srcH = 16,
		dstW = 14,
		dstH = 14,
	})
end

return BaseInvader
