--!strict

function doHelloWorld()
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

	local e2 = ECS.create()
	local p2 = Components.Position()
	p2.x = 2
	p2.y = 22
	p2.z = 222
	ECS.setPosition(e2, p2)
	local v2 = Components.Velocity()
	v2.x = 2222
	v2.y = 22222
	ECS.setVelocity(e2, v2)

	local e3 = ECS.create()
	local p3 = Components.Position()
	p3.x = 2
	p3.y = 22
	p3.z = 222
	ECS.setPosition(e3, p3)

	print(Components.Position.TypeID)
	print(Components.Velocity.TypeID)

	ECS.view({ Components.Velocity, Components.Position })(function(viewE)
		local pos = ECS.getPosition(viewE)
		print("entity " .. tostring(viewE) .. " position X is: " .. tostring(pos.x))

		local vel = ECS.getVelocity(viewE)
		print("entity " .. tostring(viewE) .. " velocity Y is: " .. tostring(vel.y))
	end)
end

function addEm(a: number, b: number): number
	return a + b
end
