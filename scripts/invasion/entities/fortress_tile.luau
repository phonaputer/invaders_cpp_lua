--!strict
local FortressTile = {}

FortressTile.DRAW_HEIGHT = 4
FortressTile.DRAW_WIDTH = 4
FortressTile.SRC_HEIGHT = 4
FortressTile.SRC_WIDTH = 4

local MAX_HITPOINTS = 3
local DAMAGE_SPRITE_OFFSET_X = 6 * FortressTile.SRC_WIDTH

local fortressTileOnDamageCallback = Callbacks.register("invasion", "Entities.Fortress.Tile.onDamage")

function FortressTile.onDamage(entity: Entity, amount: number): ()
	local offset = DAMAGE_SPRITE_OFFSET_X * amount

	local sprite = ECS.getSprite(entity)
	sprite.srcX = sprite.srcX + offset
	ECS.setSprite(entity, sprite) -- Test if this is needed or not
end

export type NewFortressTileArgs = {
	x: number,
	y: number,
	spriteSrcX: number,
	spriteSrcY: number,
}

function FortressTile.new(args: NewFortressTileArgs): ()
	local entity = ECS.create()

	local collision = Components.CollisionActive()
	collision.hitboxOffsetX = 0
	collision.hitboxOffsetY = 0
	collision.hitboxW = FortressTile.DRAW_WIDTH
	collision.hitboxH = FortressTile.DRAW_HEIGHT
	ECS.setCollisionActive(entity, collision)

	local damage = Components.Damage()
	damage.type = DamageType.FORTRESS
	damage.amount = 1
	ECS.setDamage(entity, damage)

	local damageCallback = Components.DamageCallback()
	damageCallback.callback = fortressTileOnDamageCallback
	ECS.setDamageCallback(entity, damageCallback)

	local hitpoints = Components.Hitpoints()
	hitpoints.susceptibleTo = bit32.bor(DamageType.ALIEN_PROJECTILE, DamageType.PLAYER_PROJECTILE)
	hitpoints.curHitpoints = MAX_HITPOINTS
	ECS.setHitpoints(entity, hitpoints)

	local position = Components.Position()
	position.x = args.x
	position.y = args.y
	position.w = FortressTile.DRAW_WIDTH
	position.h = FortressTile.DRAW_HEIGHT
	position.z = 20
	ECS.setPosition(entity, position)

	local sprite = Components.Sprite()
	sprite.srcID = "space_invaders.png"
	sprite.srcX = args.spriteSrcX
	sprite.srcY = args.spriteSrcY
	sprite.srcW = FortressTile.SRC_WIDTH
	sprite.srcH = FortressTile.SRC_HEIGHT
	sprite.dstW = FortressTile.DRAW_WIDTH
	sprite.dstH = FortressTile.DRAW_HEIGHT
	ECS.setSprite(entity, sprite)
end

return FortressTile
