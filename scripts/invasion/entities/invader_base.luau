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

	local collision = Components.CollisionPassive()
	collision.hitboxOffsetX = args.hitboxOffsetX
	collision.hitboxOffsetY = args.hitboxOffsetY
	collision.hitboxW = args.hitboxW
	collision.hitboxH = args.hitboxH
	ECS.setCollisionPassive(entity, collision)

	local damage = Components.Damage()
	damage.type = DamageType.ALIEN
	damage.amount = 1
	ECS.setDamage(entity, damage)

	local deletionCallback = Components.DeletionCallback()
	deletionCallback.callback = args.onDeleteCallback
	ECS.setDeletionCallback(entity, deletionCallback)

	local hitpoints = Components.Hitpoints()
	hitpoints.susceptibleTo = DamageType.PLAYER_PROJECTILE
	hitpoints.curHitpoints = 1
	ECS.setHitpoints(entity, hitpoints)

	local animation = Components.InvaderAnimation()
	animation.stripID = args.animationStrip
	ECS.setInvaderAnimation(entity, animation)

	ECS.setOrchestratedInvader(entity, Components.OrchestratedInvader())

	local position = Components.Position()
	position.x = args.x
	position.y = args.y
	position.w = 16
	position.h = 16
	position.z = 1
	ECS.setPosition(entity, position)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = 16 * args.animationFirstFrame.x
	sprite.srcY = 16 * args.animationFirstFrame.y
	sprite.srcW = 16
	sprite.srcH = 16
	sprite.dstW = 14
	sprite.dstH = 14
	ECS.setSprite(entity, sprite)
end

return BaseInvader
