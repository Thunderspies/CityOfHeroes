-- Header file required to use ALL_PLAYERS
LoadLuaFile("Lua/Util.lua")
LoadLuaFile("Lua/SZE/SZEMain.lua")

-- The purpose of this script is to play a full screen FX once on a player.

function init()

	Message.DebugPrint("Full screen FX script is activated.")

-- The designer specifies when this script should be activated in a mission.
	ActivateOnObjectiveComplete = Var.Get("ACTIVATE_ON_OBJECTIVE_COMPLETE")

-- The designer specifies what FX Should be played on the players.
	FXName = Var.Get("PLAY_FX")

end

init()

function tick()

-- When the mission objective that the designer has set up is true, then it will play the FX that the designer has specified.
	if Mission.ObjectiveIsSucceeded(ActivateOnObjectiveComplete) then
		Message.DebugPrint("Playing FX.")
		Entity.PlayFX(ALL_PLAYERS, FXName)
		EndScript()
	end

end

