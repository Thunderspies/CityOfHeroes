-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")

-- The point of the below function is to specify what special thing we want to do in the teleporter.
-- It's important to note that if you want the below to happen, you need to specify which teleporter it happens on.
-- If you don't, it will default to the regular function for teleporting players.
-- You could do a lot of fun things with this.
-- Sincerely,
-- McSean

function TeleportSpecialFX (self, player)
	if self.destination then
		Entity.PlayFX(player, "GENERIC/Fades/Pain_Blackout_Short.fx")
		Message.Floater(player, "Penelope Yin teleports you!", 0x00ff00ff, "Attention")
		Delay.delay(1, function() Team.TeleportToLocation(player, self.destination) end, false)
	end
end

-- Required to run Scripted Zone Event

tick = function() szeTick() end

-- This script is used to make teleporting volumes in missions.

-- STAGE DEFINITIONS ##################################################################################################
-- The following stages may consist of: startup, shutdown, tick.
-- Tick will take place as often as scripted zone events are updated, while the stage is active.

-- Setup Stage ----------------------------------------------------------------

setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)
	Message.DebugPrint("Starting Stage Setup for the Brickstown Event. Waiting for the objective ScriptHandleThem to go off.")

--  TELEPORTER CREATION####################################################################
--  More teleporters can be added, as long as you don't do anything to previous teleporters.
-- The first name "Teleporter", is going to be the script's internal reference to the teleport name.
-- The volume is what will volume name will teleport players away.
-- The destination is the script marker where the volume will send players.

	SZEMain:addTeleport("Teleporter", Teleport:new{ volume = "Teleporter", destination = "marker:Teleporter" })
	SZEMain.teleports["Teleporter"].teleport = TeleportSpecialFX
	SZEMain:addTeleport("TeleporterTwo", Teleport:new{ volume = "TeleporterTwo", destination = "marker:TeleporterTwo" })


-- This will put the event in the next stage.
	self.nextStage = "Teleport"


end



--  END OF SET UP STAGE###############################################################################################################################################################


setupStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Startup")
end

--  BEGINNING OF TELEPORTER STAGE STAGE###############################################################################################################################################################

stage1 = Stage:new{ name = "Teleport" }
stage1.startup = function(self)
	Message.DebugPrint("Starting Teleport Stage")
end


-- The bottom function checks every tick if the objectives are met. When ScriptTeleport is activated, it will activate the Teleporter.
-- If you want to make a teleporter deactive, then it would be SZEMain.teleports["TeleporterName"]:deactivate(0)
-- Please don't do this to existing teleporters, as previous designers may not have ever intended for their teleporters to deactivate.



stage1.tick = function(self)
	if not self.TeleportDone and Mission.ObjectiveIsSucceeded("Boss") then
		self.TeleportDone = true
		Message.DebugPrint("Teleporter is active.")
		SZEMain.teleports["Teleporter"]:activate()
	end
	if not self.Teleport2Done and Mission.ObjectiveIsSucceeded("ScriptDead") then
		self.Teleport2Done = true
		Message.DebugPrint("Teleporter Two is active.")
		SZEMain.teleports["TeleporterTwo"]:activate()
		SZEMain.teleports["Teleporter"]:deactivate()
	end
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
SZEMain:addStage("Teleport", stage1)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("Setup")
