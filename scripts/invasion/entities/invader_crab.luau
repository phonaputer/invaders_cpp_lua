local BaseInvader = require("./invader_base")

local InvaderCrab = {}

local crabStrip = AnimationStrips.create()
AnimationStrips.addFrame(crabStrip, 1, 1)
AnimationStrips.addFrame(crabStrip, 0, 1)
AnimationStrips.addFrame(crabStrip, 1, 1)
AnimationStrips.addFrame(crabStrip, 2, 1)

local crabOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.Crab.onDelete")

function InvaderCrab.onDelete(entity: Entity): ()
	BaseInvader.onDelete(entity)
	Game.incrementScore(20)
end

function InvaderCrab.new(x: number, y: number)
	BaseInvader.new({
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

return InvaderCrab
