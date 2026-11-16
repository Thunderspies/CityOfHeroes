-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZETeleport.lua")
LoadLuaFile("Lua/SZE/SZEMain.lua")
LoadLuaFile("Lua/SZE/SZEDelay.lua")


-- The point of the below function is to specify what special thing we want to do in the teleporter.
-- It's important to note that if you want the below to happen, you need to specify which teleporter it happens on.
-- If you don't, it will default to the regular function for teleporting players.
-- You could do a lot of fun things with this.
-- Sincerely,
-- McSean

	FXName = Var.Get("TELEPORT_FX")
	FloaterText = Var.Get("FLOATER_TEXT")
	VolumeName = Var.Get("VOLUME_NAME")
	Waypoint = Var.Get("TELEPORT_DESTINATION")
	ActivateObjective = Var.Get("ACTIVATE_ON_OBJECTIVE_COMPLETE")
	DeactivateObjective = Var.Get("DEACTIVATE_ON_OBJECTIVE_COMPLETE")


function TeleportSpecialFX (self, player)
	if self.destination then
		Entity.PlayFX(player, FXName)
		Message.Floater(player, FloaterText, 0x00ff00ff, "Attention")
		Message.DebugPrint(player)
		Delay.delay(1, function() Team.TeleportToLocation(player, self.destination) end, false, DelayList)
	end
end

function OnEnterTeleportVolume (player, volume)
	if volume == VolumeName then
		Teleporter:teleport(player)
	end
end

-- Required to run Scripted Zone Event



-- This script is used to make teleporting volumes in missions.

function init()
	Message.DebugPrint("Starting the teleport special FX script.")

--  TELEPORTER CREATION####################################################################
--  More teleporters can be added, as long as you don't do anything to previous teleporters.
-- The first name "Teleporter", is going to be the script's internal reference to the teleport name.
-- The volume is what will volume name will teleport players away.
-- The destination is the script marker where the volume will send players.

	Teleporter = Teleport:new{ volume = VolumeName, destination = Waypoint }
	Teleporter.teleport = TeleportSpecialFX

	DelayList = {}
	
end

init()

function tick()

	Delay.processDelayedActions(DelayList)

	if not TeleportDone and Mission.ObjectiveIsSucceeded(ActivateObjective) then
		TeleportDone = true
		Teleporter:activate()
		Message.DebugPrint("Teleporter is active.")
		Callback.OnPlayerEntersVolume("OnEnterTeleportVolume")
	end

	if Mission.ObjectiveIsSucceeded(DeactivateObjective) then
		Teleporter:deactivate()
		Message.DebugPrint("Deactivate Objective succeeded. Ending script.")
		EndScript()
	end

end

