local s = "Hello! Now I will count to eleven... 1"

for i = 2, 11 do
	s = s .. " " .. i
end

test.print_from_cpp(s .. "!")
