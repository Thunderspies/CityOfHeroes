-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end





Script.SkyFileFade(Script.SkyFileGetByName("sun_Faultline_NewOverbrook.txt"), Script.SkyFileGetByName("sun_Faultline_NewOverbrook.txt"), 1.0)


stage1 = Stage:new{ name = "Startup" }
stage1.startup = function(self)
	Message.DebugPrint("Waiting to change skies.")
	SZEMain:addTimer("SkyTimer", Timer:new{ duration = 1 }, true)
	SZEMain.timers["SkyTimer"]:stop()
end
-- This says that the following should be refreshed at every tick.
stage1.tick = function(self)

	if not self.ExplodeDone and Mission.ObjectiveIsSucceeded("Bombs") then
		Entity.PlayFX(player, "WORLD/City/Explosion_FullScreen.fx")
		self.ExplodeDone = true
		SZEMain.timers["SkyTimer"]:start()
	end

-- BE AWARE OF THE TWO!
	if not self.SkyFaded and SZEMain.timers["SkyTimer"]:complete() then
		Message.DebugPrint("Changing skies.")
		self.SkyFaded = true
		Script.SkyFileFade(Script.SkyFileGetByName("sun_Faultline_NewOverbrook.txt"), Script.SkyFileGetByName("sun_outdoor_fire.txt"), 1.0)
	end

end
-- Name the Event
SZEMain.eventName = "StMartial_arc_3_mission_4_SkyFade"

-- Add Stages listed above
SZEMain:addStage("startup", stage1)

-- Define stage to start
SZEMain:gotoStage("startup")
