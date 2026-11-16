-- Header file required to use ALL_PLAYERS
LoadLuaFile("Lua/Util.lua")
--LoadLuaFile("Lua/SZE/SZETimer.lua")

-- Call this function in your script within the tick function

function ReviveCheck(Boss,DeadDialog,RecoverDialog,RespawnTimer)

-- This refreshes the variable currenthealth every tick to equal the entity's health who has been set as boss.
	currentHealth = Entity.GetHealth(Boss)

	--Initialize timer and run pop help on first loop
	if not RunOnce then
		Message.PopHelp(ALL_PLAYERS, "POP_HELP_ALLIES")
		RunOnce = true
	end

	if currentHealth == 0 and not RespawnTimer:running() then
		Message.DebugPrint("Ally has died.")
		Entity.Say(Boss, DeadDialog	, 2)
		RespawnTimer:start()
	end

	if RespawnTimer:complete() then
		Message.DebugPrint("Ally is alive.")
		Entity.SetHealth(Boss, 1, false)
		Entity.Say(Boss, RecoverDialog	, 2)
		RespawnTimer:stop()
	end

end

