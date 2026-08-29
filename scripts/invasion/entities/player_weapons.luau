local PlayerWeapons = {}

local PlayerProjectile = require("./player_projectile")
local PlayerMuzzleFlash = require("./player_muzzle_flash")

function PlayerWeapons.defaultAttack(player_entity: number, x: number, y: number): ()
	if PlayerProjectile.getActiveCount() < 3 then
		PlayerProjectile.new(x, y)
		PlayerMuzzleFlash.new(player_entity)
		Game.playSound("player_shot.wav")
	end
end

local defaultWeaponCallback = Callbacks.register("invasion", "Entities.Player.Weapons.defaultAttack")

PlayerWeapons.Default = Weapons.register({
	ticksPerShot = 8,
	shootCallback = defaultWeaponCallback,
})

function PlayerWeapons.fastshotAttack(player_entity: number, x: number, y: number): ()
	PlayerProjectile.new(x, y)
	PlayerMuzzleFlash.new(player_entity)
	Game.playSound("player_shot.wav")
end

local fastshotWeaponCallback = Callbacks.register("invasion", "Entities.Player.Weapons.fastshotAttack")

PlayerWeapons.Fastshot = Weapons.register({
	ticksPerShot = 4,
	shootCallback = fastshotWeaponCallback,
})

return PlayerWeapons
