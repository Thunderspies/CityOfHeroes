-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZETeleport.lua")


-- This script is used to make teleporting volumes in missions.
-- This is an older script. Designers should use the Teleport_SpecialFX script.


function init()
	Message.DebugPrint("Starting the teleport script.")

--  TELEPORTER CREATION####################################################################
--  More teleporters can be added, as long as you don't do anything to previous teleporters.
-- The first name "Teleporter", is going to be the script's internal reference to the teleport name.
-- The volume is what will volume name will teleport players away.
-- The destination is the script marker where the volume will send players.

	Teleporter = Teleport:new{ volume = "Teleporter", destination = "marker:Teleporter" }

-- This will put the event in the next stage.

end

init()

function tick()

	if not self.TeleportDone and Mission.ObjectiveIsSucceeded("ScriptTeleport") then
		self.TeleportDone = true
		Message.DebugPrint("Teleporter is active.")
		Teleporter:activate()
	end
end


