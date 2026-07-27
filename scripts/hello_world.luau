--!strict

local e = ECS.create()

local p = Components.Position()

p.x = 1
p.y = 2
p.z = 3

print("has position, before set: " .. tostring(ECS.hasPosition(e)))

ECS.setPosition(e, p)

print("has position, after set: " .. tostring(ECS.hasPosition(e)))

test.printPosition(ECS.getPosition(e))
