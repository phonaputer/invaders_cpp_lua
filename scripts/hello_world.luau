local s = "Hello! Now I will count to twelve... 1"

for i = 2, 12 do
	s = s .. " " .. i
end

test.print_from_cpp(s .. "!")
