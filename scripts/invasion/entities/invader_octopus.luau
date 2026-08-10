local BaseInvader = require("./invader_base")

local InvaderOctopus = {}

local octopusStrip = AnimationStrips.create()
AnimationStrips.addFrame(octopusStrip, 6, 0)
AnimationStrips.addFrame(octopusStrip, 7, 0)

local octopusOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.Octopus.onDelete")

function InvaderOctopus.onDelete(entity: Entity): ()
	BaseInvader.onDelete(entity)
	Game.incrementScore(40)
end

function InvaderOctopus.new(x: number, y: number)
	BaseInvader.new({
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

return InvaderOctopus
