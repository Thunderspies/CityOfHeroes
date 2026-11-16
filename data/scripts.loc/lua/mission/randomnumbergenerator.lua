-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- This script is used to spawn glowies in a mission that will have extra items for players.


-- STAGE DEFINITIONS ##################################################################################################
-- The following stages may consist of: startup, shutdown, tick.
-- Tick will take place as often as scripted zone events are updated, while the stage is active.

-- Setup Stage ----------------------------------------------------------------

setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)
	Message.DebugPrint("Starting stage set up for the Random Number Generator. If you don't want the RNG to run, go to your console and do rwtoken TurnOffRNG 1. This will stop the random number generator from working.")

-- This helps the RNG be truly random.
	math.randomseed(os.time())

-- This part is for QA.
-- If they have rewarded themself with the token to turn off the RNG, then the script will end.
-- This is so QA can turn off the script and set the objectives manually.
	if Reward.HasToken(ALL_PLAYERS, "TurnOffRNG") then
		Message.DebugPrint("You have the token, TurnOffRNG, so now the RNG is turning off.")
		EndScript()
		return
	end

-- This will put the event in the next stage.
	self.nextStage = "Roll"

end



--  END OF SET UP STAGE###############################################################################################################################################################


setupStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Startup")
end

--  BEGINNING OF REWARD STAGE###############################################################################################################################################################

stage1 = Stage:new{ name = "Roll" }
stage1.startup = function(self)
	Message.DebugPrint("Starting Roll Stage")

-- The designer sets up the chance number, which is between 0 and 100.
-- The designer specifies the objectives to complete when the chance succeeds and fails.
	Chance = Var.GetNumber( "Chance" )
	ObjectiveCompleteOnSucceed = Var.Get( "ObjectiveCompleteOnSucceed" )
	ObjectiveCompleteOnFail = Var.Get( "ObjectiveCompleteOnFail" )

-- This sets up the variable mynumber, which is the random roll between 0 and 100.
	mynumber = math.random(0, 100)
	Message.DebugPrint(mynumber)

-- This checks to see if mynumber is less than the number in Chance, specified by the designer. If not, it will run the ELSE part, which is if the number is greater.
-- Both methods will stop the script from looping.
	if mynumber <= Chance then
		Message.DebugPrint("My number is less than the amount. Setting the objective in ObjectiveCompleteOnSucceed to be true and shutting off script.")
		Mission.SetObjectiveComplete(1, ObjectiveCompleteOnSucceed)
		EndScript()
	else
		Message.DebugPrint("My number is more than the amount. Setting the objective in ObjectiveCompleteOnFail to be true and shutting off script.")
		Mission.SetObjectiveComplete(1, ObjectiveCompleteOnFail)
		EndScript()
	end

end




stage1.tick = function(self)
end

--  ###############################################################################################################################################################

-- Stop Stage: ----------------------------------------------------------------
stopStage = Stage:new{ name = "Stop" }
stopStage.startup = function(self)
	Message.DebugPrint("Starting Stage Stop")

	EndScript()
end
-- End STAGE DEFINITIONS ##############################################################################################

-- Name the Event
SZEMain.eventName = "Teleport"

-- Add Stages listed above
SZEMain:addStage("Setup", setupStage)
SZEMain:addStage("Roll", stage1)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("Setup")
