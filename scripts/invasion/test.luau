local test2 = require("./test_two")

local Module = {}

function Module.add(x: number, y: number): number
	return test2.add(x, y)
end

return Module
