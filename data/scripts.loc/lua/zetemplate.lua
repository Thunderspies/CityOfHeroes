-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Requried to run Scripted Zone Event
tick = function() szeTick() end

-- HELPER FUNCTIONS ###################################################################################################
-- Add helper functions to simplify syntax or pull out logic that may be reused.

-- End HELPER FUNCTIONS ###############################################################################################

-- STAGE DEFINITIONS ##################################################################################################
-- The following stages may consist of: startup, shutdown, tick.
-- Tick will take place as often as scripted zone events are updated, while the stage is active.

-- Setup Stage ----------------------------------------------------------------
setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)
	Message.DebugPrint("Starting Stage Setup")
	Message.DebugPrint("This is an 8-16 player event")
	
	-- Immediately progress to next stage when setup is done.
	self.nextStage = "Stop"
end
setupStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Startup")
end

-- Stop Stage: ----------------------------------------------------------------
stopStage = Stage:new{ name = "Stop" }
stopStage.startup = function(self)
	Message.DebugPrint("Starting Stage Stop")
end
-- End STAGE DEFINITIONS ##############################################################################################

-- Name the Event
SZEMain.eventName = "Zone Event Template"

-- Add Stages listed above
SZEMain:addStage("Setup", setupStage)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("Setup")