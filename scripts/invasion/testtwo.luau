local Result = {}

Result.Example = 0

function Result.IncrExample(): ()
	Result.Example += 1
	print(Result.Example)
end

return Result
