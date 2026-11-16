-- We're loading this to get the functionality from Util.lua
LoadLuaFile("Lua/Util.lua")

init = function()
	Message.DebugPrint("Reward script is running.")

	-- This is the script that handles handing out rewards to players on a mission team.
	-- Reward = Var.Get( "Reward" )
	-- Loadstring is going to convert text into code. This is used for when you're doing character evaluator statements.
	-- Instead of it being seen as, "TaskOwner?" and the code is all like, "What the hell do you mean?"
	-- It will be seen as, TaskOwner? and the code will give you the thumb's up and process that.
	-- CaptionRequires = loadstring(Var.Get("Requires"))

	-- The Var.ArrayAccount is getting how many variables we have in the Requires varlist. This will be doing the same for the Caption down below.
	-- Arrays are called tables in Lua. You define the table with the {}. So capList= {} is an empty table.
	-- All this is doing is VarArray is getting from the row that you're getting it from, and the i is the index/how many variables you have.
	-- If you have 5 requires, i would go from 1 to 5. So 1... 2... 3... 4... 5...
	RequireList = {}
	for i = 1, Var.ArrayCount("Requires") do
		preString = "local player = select(1,...) return "
		-- local requireFunc = function(player) return (player) end
		table.insert(RequireList, assert(loadstring(preString .. Var.ArrayGet("Requires", i))))
	end

	RewardList = {}
	for i = 1, Var.ArrayCount("Reward") do
		table.insert(RewardList, Var.ArrayGet("Reward", i))
	end

	RewardCompleteList = {}
end

init()

tick = function()
	local allComplete = true
-- This says, "for i=1 to whatever number has been set by the RequiresList. The # says, "How many entries are in the table listed after me."
	for i=1, #RequireList do
		if not RewardCompleteList[i] then
			allComplete = false
			for j = 1, Team.NumEntitiesInTeam(ALL_PLAYERS) do
				player = Team.GetEntityFromTeam(ALL_PLAYERS, j)
				if RequireList[i](player) then
					Reward.GrantToMissionTeam(RewardList[i])
					RewardCompleteList[i] = true
					break
				end
			end
		end
	end
	if allComplete then
		EndScript()
	end
end
