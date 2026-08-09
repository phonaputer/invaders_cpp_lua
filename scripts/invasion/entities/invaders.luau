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
	if math.random(3) ~= 1 then
		return
	end

	local invader, ok = getRandomInvader()
	if not ok then
		return
	end

	local position = ECS.getPosition(invader)
	local xMidpoint = position.x + (position.w / 2)
	local xJitter = math.random(7) - 4
	newInvaderProjectile(xMidpoint + xJitter, position.y)
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

local activeInvaders: { [Entity]: boolean } = {}
local activeInvaderCount = 0

function getRandomInvader(): (Entity, boolean)
	if activeInvaderCount < 1 then
		return 0, false
	end

	local selected = math.random(activeInvaderCount)
	local counter = 1
	for entity, _ in activeInvaders do
		if counter == selected then
			return entity, true
		end
		counter += 1
	end

	return 0, false
end

function onDeleteInvader(entity: Entity): ()
	local position = ECS.getPosition(entity)
	newExplosion(position.x, position.y)

	activeInvaders[entity] = nil
	activeInvaderCount -= 1
end

function newInvader(args: NewInvaderArgs)
	local entity = ECS.create()

	activeInvaders[entity] = true
	activeInvaderCount += 1

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

local projectileAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(projectileAnimationStrip, 5, 2)
AnimationStrips.addFrame(projectileAnimationStrip, 6, 2)

local projectileOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.onDeleteProjectile")

function Invaders.onDeleteProjectile(entity: Entity): ()
	local position = ECS.getPosition(entity)
	newExplosion(position.x, position.y)
end

function newInvaderProjectile(x: number, y: number): ()
	local entity = ECS.create()

	local animation = Components.Animation()
	animation.ticksPerFrame = 40
	animation.stripID = projectileAnimationStrip
	ECS.setAnimation(entity, animation)

	local collision = Components.CollisionActive()
	collision.hitboxOffsetX = 7
	collision.hitboxOffsetY = 7
	collision.hitboxW = 1
	collision.hitboxH = 3
	ECS.setCollisionActive(entity, collision)

	local damage = Components.Damage()
	damage.type = DamageType.ALIEN_PROJECTILE
	damage.amount = 1
	ECS.setDamage(entity, damage)

	local deletionCallback = Components.DeletionCallback()
	deletionCallback.callback = projectileOnDeleteCallback
	ECS.setDeletionCallback(entity, deletionCallback)

	local hitpoints = Components.Hitpoints()
	hitpoints.susceptibleTo = bit32.bor(DamageType.PLAYER, DamageType.PLAYER_PROJECTILE, DamageType.FORTRESS)
	hitpoints.curHitpoints = 1
	ECS.setHitpoints(entity, hitpoints)

	local position = Components.Position()
	position.x = x
	position.y = y
	position.w = 16
	position.h = 16
	position.z = 80
	ECS.setPosition(entity, position)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = 80
	sprite.srcY = 32
	sprite.srcW = 16
	sprite.srcH = 16
	sprite.dstW = 16
	sprite.dstH = 16
	ECS.setSprite(entity, sprite)

	local velocity = Components.Velocity()
	velocity.y = 1
	ECS.setVelocity(entity, velocity)
end

local explosionAnimationStrip = AnimationStrips.create()
AnimationStrips.addFrame(explosionAnimationStrip, 3, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 4, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 5, 1)
AnimationStrips.addFrame(explosionAnimationStrip, 6, 1)

function newExplosion(x: number, y: number)
	local entity = ECS.create()

	local animation = Components.Animation()
	animation.ticksPerFrame = 5
	animation.stripID = explosionAnimationStrip
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
