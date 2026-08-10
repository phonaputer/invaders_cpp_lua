--!strict
local InvaderTadpole = {}

local BaseInvader = require("./invader_base")

local tadpoleStrip = AnimationStrips.create()
AnimationStrips.addFrame(tadpoleStrip, 4, 0)
AnimationStrips.addFrame(tadpoleStrip, 3, 0)
AnimationStrips.addFrame(tadpoleStrip, 4, 0)
AnimationStrips.addFrame(tadpoleStrip, 5, 0)

local tadpoleOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.Tadpole.onDelete")

function InvaderTadpole.onDelete(entity: Entity): ()
	BaseInvader.onDelete(entity)
	Game.incrementScore(10)
end

function InvaderTadpole.new(x: number, y: number)
	BaseInvader.new({
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

return InvaderTadpole
