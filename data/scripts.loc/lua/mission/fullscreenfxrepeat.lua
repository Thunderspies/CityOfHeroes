-- Header file required to use ALL_PLAYERS
LoadLuaFile("Lua/Util.lua")
LoadLuaFile("Lua/SZE/SZETimer.lua")

-- The purpose of this script is to play a full screen FX on the player over and over again over an interval of time.

function init()

	Message.DebugPrint("Full screen repeating FX script is activated.")

-- The designer sets up what starts the script, what objective finishes when the script is complete, and the objective that can stop the script.
	ActivateOnObjectiveComplete = Var.Get("ACTIVATE_ON_OBJECTIVE_COMPLETE")
	ObjectiveCompleteOnFinish = Var.Get("OBJECTIVE_COMPLETE_ON_FINISH")
	ObjectiveToStopFX = Var.Get("DEACTIVATE_OBJECTIVE")

-- The designer sets up how long the timer, TimerforFX, is.
	TimeBetweenFX = Var.GetNumber("TIME_BETWEEN_FX")

-- The designer says how many times this script should repeat itself.
	RepeatNumber = Var.GetNumber("REPEAT_AMOUNT")

-- This is the FX that the designer specifies to play.
	FXName = Var.Get("PLAY_FX")

-- This is setting the variables RepeatCount and StopPlayinFX to zero. Maybe this isn't needed?
	RepeatCount = 0
	StopPlayingFX = 0

-- This is the timer used for repeating the FX. The duration is dependant upon what the designer says TimeBetweenFX is.
	TimeForFX = Timer:new{ duration = TimeBetweenFX}

end

init()

function tick()

-- This plays the first FX. StopPlayingFX is used in order to make sure this doesn't loop over and over again.
	if StopPlayingFX == 0 and Mission.ObjectiveIsSucceeded(ActivateOnObjectiveComplete) then
		StopPlayingFX = StopPlayingFX + 1
		Message.DebugPrint("Starting full screen FX script.")
		Entity.PlayFX(ALL_PLAYERS, FXName)
		TimeForFX:start()
	end

-- This is what happens every time the timer finishes. It adds to the RepeatCount counter, plays the FX on the players, inits the timer, and starts the timer again.
	if TimeForFX:complete() and Mission.ObjectiveIsSucceeded(ActivateOnObjectiveComplete) then
		Entity.PlayFX(ALL_PLAYERS, FXName)
		RepeatCount = RepeatCount + 1
		Message.DebugPrint("FX is playing.")
		TimeForFX:init()
		TimeForFX:start()
	end

-- When RepeatCount is equal to RepeatNumber, the variable set by the designer, then the script stops itself.
	if RepeatCount == RepeatNumber then
		Mission.SetObjectiveComplete(1, ObjectiveCompleteOnFinish)
		Message.DebugPrint("Repeat count has reached its max number. Shutting down script.")
		EndScript()
	end

-- If the mission objective that the designer specifies is succeeded, then the script will stop itself.
	if Mission.ObjectiveIsSucceeded(ObjectiveToStopFX) then
		Message.DebugPrint("Objective to stop the FX has succeeded. Shutting down the script")
		EndScript()
	end

end

