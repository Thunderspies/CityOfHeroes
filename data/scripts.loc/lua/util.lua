math.randomseed( os.time() )
OFF_IN_LEFT_FIELD = "coord:1000000.0,0.0,0.0"
STATE_VAR = "__State"
STATE_ENTRY = "__StateEntry"
STATE_TIME = "__StateStarted"
STATE_INITIAL =	"__Initial"
TEAM_NONE = "ent:none"
ALL_PLAYERS = "each:player"
ALL_PLAYERS_READYORNOT = "each:player_readyornot"
ALL_CRITTERS = "each:critter"
ALL_DOORS = "each:door"
LOC_ORIGIN = "coord:0.0,0.0,0.0"
tempCreationTeam = ".Temp.Creation.Team."
forceCreationTeam = ".Temp.ForceCreation.Team"

function deepcopy(object)
	local lookup_table = {}
	local function _copy(object)
		if type(object) ~= "table" then
			return object
		elseif lookup_table[object] then
			return lookup_table[object]
		end
		local new_table = {}
		lookup_table[object] = new_table
		for index, value in pairs(object) do
			new_table[_copy(index)] = _copy(value)
		end
		return setmetatable(new_table, getmetatable(object))
	end
	return _copy(object)
end

function pairsByKeys (t, f)
	local a = {}
	for n in pairs(t) do table.insert(a, n) end
	table.sort(a, f)
	local i = 0      -- iterator variable
	local iter = function ()   -- iterator function
		i = i + 1
		if a[i] == nil then return nil
		else return a[i], t[a[i]]
		end
	end
	return iter
end

function postpendLetters(t, s, n)
	for i = 1, n do
		table.insert(t, postpendLetter(s, i))
	end
end

function postpendLetter(s, n)
	n = n-1
	if n >= 26 then
		if n / 26 > 26 then
			return nil
		end
		return s .. string.char(64 + math.floor(n / 26), 65 + math.floor(n % 26))
	else
		return s .. string.char(65 + n)
	end
end