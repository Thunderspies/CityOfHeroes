-- Header file for general things. This should almost always be loaded into the map.
LoadLuaFile("Lua/Util.lua")

-- This script is for mission designers to call a piece of music to fade out.

init = function()
	Message.DebugPrint("Stop music script is running.")

-- The mission designer specifies the amount of time the music fades out and the objective that triggers this fade.
	FadeTime = Var.GetNumber("FadeTime")
	FadeMusicOnObjective = Var.Get("FadeOnObjectiveComplete")

end

init()

tick = function()


-- When the objective that is specified is complete, the music fades depending on the variable set up in FadeTime and then the script stops.
	if Mission.ObjectiveIsSucceeded(FadeMusicOnObjective) then
		Message.FadeSound(ALL_PLAYERS, "Music", FadeTime)
		Message.DebugPrint("Fading music, stopping script.")
		EndScript()
	end
end
