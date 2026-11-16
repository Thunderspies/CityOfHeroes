-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- This script is used to spawn glowies in a mission that will have extra items for players.
-- THIS IS AN OLDER SCRIPT THAT WAS USED TO TEST TO SEE IF THIS WORKS.

-- STAGE DEFINITIONS ##################################################################################################
-- The following stages may consist of: startup, shutdown, tick.
-- Tick will take place as often as scripted zone events are updated, while the stage is active.

-- Setup Stage ----------------------------------------------------------------

setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)
	Message.DebugPrint("Starting stage set up for the Random Loot - NORMAL script.")

-- This helps the RNG be truly random.
	math.randomseed(os.time())

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
	Message.DebugPrint("Starting Roll st age Stage")

	mynumber = math.random(1, 200)
	Message.DebugPrint(mynumber)

	if mynumber <= 25 then
	Message.DebugPrint("Mynumber is less than the amount. Setting ScriptSpawnNormalLoot to be true.")
	Mission.SetObjectiveComplete(1, "ScriptSpawnNormalLoot")
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
