--!strict

local c = Components.Position()

c.x = 1
c.y = 2
c.z = 3

test.print_from_cpp(c)
test.type_check(Components.Position.GetTypeID())
