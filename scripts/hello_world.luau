--!strict

function doHelloWorld()
	print("Hello, world")

	local e1 = ECS.create()
	local p1 = Components.Position()
	p1.x = 1
	p1.y = 11
	p1.z = 111
	ECS.setPosition(e1, p1)
	local v1 = Components.Velocity()
	v1.x = 1111
	v1.y = 11111
	ECS.setVelocity(e1, v1)

	Host.printPosition(ECS.getPosition(e1))
end

function addEm(a: number, b: number): number
	return a + b
end
