--!strict

local Invaders = {}

function Invaders.rerack(): ()
	local STARTING_X = 2
	local curX = STARTING_X
	local curY = 32
	local ALIEN_WIDTH = 14
	local ALIEN_HEIGHT = 14
	local COL_SPACING = 2
	local ROW_SPACING = -1.2

	local rowInvaderCreationFuncs = {
		newOctopus,
		newJellyfish,
		newCrab,
		newTadpole,
		newTadpole,
	}

	for _, createInvader in rowInvaderCreationFuncs do
		for i = 1, 11 do
			createInvader(curX, curY)
			curX += ALIEN_WIDTH + COL_SPACING
		end

		curX = STARTING_X
		curY += ALIEN_HEIGHT + ROW_SPACING
	end
end

function Invaders.shoot(): ()
	print("pow")
end

function onDeleteInvader(entity: Entity): ()
	local position = ECS.getPosition(entity)
	newExplosion(position.x, position.y)
end

local octopusStrip = AnimationStrips.create()
AnimationStrips.addFrame(octopusStrip, 6, 0)
AnimationStrips.addFrame(octopusStrip, 7, 0)

local octopusOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.onDeleteOctopus")

function Invaders.onDeleteOctopus(entity: Entity): ()
	onDeleteInvader(entity)
	HUD.incrementScore(40)
end

function newOctopus(x: number, y: number)
	newInvader({
		x = x,
		y = y,
		animationStrip = octopusStrip,
		animationFirstFrame = { x = 6, y = 0 },
		onDeleteCallback = octopusOnDeleteCallback,
		hitboxOffsetX = 0,
		hitboxOffsetY = 3,
		hitboxW = 14,
		hitboxH = 8,
	})
end

local jellyfishStrip = AnimationStrips.create()
AnimationStrips.addFrame(jellyfishStrip, 1, 0)
AnimationStrips.addFrame(jellyfishStrip, 0, 0)
AnimationStrips.addFrame(jellyfishStrip, 1, 0)
AnimationStrips.addFrame(jellyfishStrip, 2, 0)

local jellyfishOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.onDeleteJellyfish")

function Invaders.onDeleteJellyfish(entity: Entity): ()
	onDeleteInvader(entity)
	HUD.incrementScore(30)
end

function newJellyfish(x: number, y: number)
	newInvader({
		x = x,
		y = y,
		animationStrip = jellyfishStrip,
		animationFirstFrame = { x = 1, y = 0 },
		onDeleteCallback = jellyfishOnDeleteCallback,
		hitboxOffsetX = 1,
		hitboxOffsetY = 3,
		hitboxW = 12,
		hitboxH = 8,
	})
end

local crabStrip = AnimationStrips.create()
AnimationStrips.addFrame(crabStrip, 1, 1)
AnimationStrips.addFrame(crabStrip, 0, 1)
AnimationStrips.addFrame(crabStrip, 1, 1)
AnimationStrips.addFrame(crabStrip, 2, 1)

local crabOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.onDeleteCrab")

function Invaders.onDeleteCrab(entity: Entity): ()
	onDeleteInvader(entity)
	HUD.incrementScore(20)
end

function newCrab(x: number, y: number)
	newInvader({
		x = x,
		y = y,
		animationStrip = crabStrip,
		animationFirstFrame = { x = 1, y = 1 },
		onDeleteCallback = crabOnDeleteCallback,
		hitboxOffsetX = 2,
		hitboxOffsetY = 2,
		hitboxW = 10,
		hitboxH = 9,
	})
end

local tadpoleStrip = AnimationStrips.create()
AnimationStrips.addFrame(tadpoleStrip, 4, 0)
AnimationStrips.addFrame(tadpoleStrip, 3, 0)
AnimationStrips.addFrame(tadpoleStrip, 4, 0)
AnimationStrips.addFrame(tadpoleStrip, 5, 0)

local tadpoleOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.onDeleteTadpole")

function Invaders.onDeleteTadpole(entity: Entity): ()
	onDeleteInvader(entity)
	HUD.incrementScore(10)
end

function newTadpole(x: number, y: number)
	newInvader({
		x = x,
		y = y,
		animationStrip = tadpoleStrip,
		animationFirstFrame = { x = 4, y = 0 },
		onDeleteCallback = tadpoleOnDeleteCallback,
		hitboxOffsetX = 4,
		hitboxOffsetY = 2,
		hitboxW = 5,
		hitboxH = 9,
	})
end

type NewInvaderArgs = {
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

type Frame = {
	x: number,
	y: number,
}

function newInvader(args: NewInvaderArgs)
	local entity = ECS.create()

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

function newExplosion(x: number, y: number)
	local entity = ECS.create()

	local strip = AnimationStrips.create()
	AnimationStrips.addFrame(strip, 3, 1)
	AnimationStrips.addFrame(strip, 4, 1)
	AnimationStrips.addFrame(strip, 5, 1)
	AnimationStrips.addFrame(strip, 6, 1)
	local animation = Components.Animation()
	animation.ticksPerFrame = 5
	animation.stripID = strip
	ECS.setAnimation(entity, animation)

	local position = Components.Position()
	position.x = x
	position.y = y
	position.w = 16
	position.h = 16
	position.z = 200
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
	ttl.ticksToLive = 20
	ECS.setTTL(entity, ttl)
end

return Invaders
