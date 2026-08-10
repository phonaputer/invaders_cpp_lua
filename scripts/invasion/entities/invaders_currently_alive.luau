--!strict
local InvadersCurrentlyAlive = {}

local invaders: { [Entity]: boolean } = {}
local invaderCount = 0

function InvadersCurrentlyAlive.add(entity: Entity): ()
	invaders[entity] = true
	invaderCount += 1
end

function InvadersCurrentlyAlive.remove(entity: Entity): ()
	invaders[entity] = nil
	invaderCount -= 1
end

function InvadersCurrentlyAlive.clearAll(): ()
	invaders = {}
	invaderCount = 0
end

function InvadersCurrentlyAlive.getRandom(): (Entity, boolean)
	if invaderCount < 1 then
		return 0, false
	end

	local selected = math.random(invaderCount)
	local counter = 1
	for entity, _ in invaders do
		if counter == selected then
			return entity, true
		end
		counter += 1
	end

	return 0, false
end

return InvadersCurrentlyAlive
