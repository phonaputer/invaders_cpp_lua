local BaseInvader = require("./invader_base")

local InvaderJellyfish = {}

local jellyfishStrip = AnimationStrips.create()
AnimationStrips.addFrame(jellyfishStrip, 1, 0)
AnimationStrips.addFrame(jellyfishStrip, 0, 0)
AnimationStrips.addFrame(jellyfishStrip, 1, 0)
AnimationStrips.addFrame(jellyfishStrip, 2, 0)

local jellyfishOnDeleteCallback = Callbacks.register("invasion", "Entities.Invaders.Jellyfish.onDelete")

function InvaderJellyfish.onDelete(entity: Entity): ()
	BaseInvader.onDelete(entity)
	Game.incrementScore(30)
end

function InvaderJellyfish.new(x: number, y: number)
	BaseInvader.new({
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

return InvaderJellyfish
