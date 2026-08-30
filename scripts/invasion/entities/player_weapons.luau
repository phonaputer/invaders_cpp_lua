local PlayerWeapons = {}

local PlayerBomb = require("./player_bomb")
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

function PlayerWeapons.bombAttack(player_entity: number, x: number, y: number): ()
	PlayerBomb.newProjectile(x, y)
	PlayerMuzzleFlash.new(player_entity)
	Game.playSound("player_shot.wav")
end

local bombWeaponCallback = Callbacks.register("invasion", "Entities.Player.Weapons.bombAttack")

PlayerWeapons.Bomb = Weapons.register({
	ticksPerShot = 50,
	shootCallback = bombWeaponCallback,
})

return PlayerWeapons
