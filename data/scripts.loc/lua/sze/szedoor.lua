SZEDoor = {
	lock = function(door)
		SZEMain.doors[door] = false
		if Location.Exists("marker:" .. door) then
			Door.Lock("marker:" .. door)
		end
	end,
	unlock = function(door)
		SZEMain.doors[door] = true
		if Location.Exists("marker:" .. door) then
			Door.Unlock("marker:" .. door)
		end
	end }
		