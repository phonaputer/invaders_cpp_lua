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

	ECS.setCollisionActive(entity, {
		hitboxOffsetX = 0,
		hitboxOffsetY = 0,
		hitboxW = FortressTile.DRAW_WIDTH,
		hitboxH = FortressTile.DRAW_HEIGHT,
	})

	ECS.setDamage(entity, {
		type = DamageType.FORTRESS,
		amount = 1,
	})

	ECS.setDamageCallback(entity, {
		callback = fortressTileOnDamageCallback,
	})

	ECS.setHitpoints(entity, {
		susceptibleTo = bit32.bor(DamageType.ALIEN_PROJECTILE, DamageType.PLAYER_PROJECTILE),
		curHitpoints = MAX_HITPOINTS,
	})

	ECS.setPosition(entity, {
		x = args.x,
		y = args.y,
		w = FortressTile.DRAW_WIDTH,
		h = FortressTile.DRAW_HEIGHT,
		z = 20,
	})

	ECS.setSprite(entity, {
		srcID = "space_invaders.png",
		srcX = args.spriteSrcX,
		srcY = args.spriteSrcY,
		srcW = FortressTile.SRC_WIDTH,
		srcH = FortressTile.SRC_HEIGHT,
		dstW = FortressTile.DRAW_WIDTH,
		dstH = FortressTile.DRAW_HEIGHT,
	})
end

return FortressTile
