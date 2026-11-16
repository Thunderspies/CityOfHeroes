-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- This script is set up to change the sky in the area when an objective is complete.
-- The script is also set up to change the UI based on objectives being complete in a mission.

-- This counter is used when you want to have the UI count up something.
countGroupStatus = function(Status, increase, timerName)
	if SZEMain.timers[timerName]:complete() then
		Status = Status + increase
		SZEMain.timers[timerName]:init()
	end
	return Status
end

-- This counter is used when you want to have an enemy health be displayed instead. It's neat!
countGroupHealth = function(Status, damage, timerName)
	if SZEMain.timers[timerName]:complete() then
		Status = Status - damage
		SZEMain.timers[timerName]:init()
	end
	return Status
end

-- This is here in order to make sure that the sky file is the default one in the mission, just to be safe.
		Script.SkyFileFade(Script.SkyFileGetByName("Sky_Praetoria_FirstWard_A.txt"), Script.SkyFileGetByName("Sky_Praetoria_FirstWard_A.txt"), 1.0)


stage1 = Stage:new{ name = "Startup" }
stage1.startup = function(self)
	Message.DebugPrint("Starting Stage 1 the St. Martial event")

	SZEMain:addTimer("Group1Timer", Timer:new{ duration = 15 }, true)
	SZEMain:addTimer("Group2Timer", Timer:new{ duration = 10 }, true)
	SZEMain:addTimer("Group3Timer", Timer:new{ duration = 5 }, true)

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					-- "Stage1" refers to the UICollection from Zone Events.
					name = "Stage1",
					-- ScriptDisplayName is DisplayName that shows up on top of the UI.
					scriptDisplayName = "The New Praetorians",
					-- StagedisplayName is the text that shows up inside of the actual UI.
					stageDisplayName = "Chasing Leads",
					items =
					{
						UIItem:new
						{
							name = "Group1Text",
							uiType = "Text",
							text = "Pendragon and Aurora are fighting Freakshow.",
							display = function() return not Mission.ObjectiveIsSucceeded("Script10") end
						} ,
						UIItem:new
						{
							name = "Group1Bar",
							uiType = "BarBlue",
							text = "Freakshow arrested:",
							currentVal = function() return status1 end,
							targetVal = function() return 30 end,
							display = function() return not Mission.ObjectiveIsSucceeded("Script10") end
						} ,
						UIItem:new
						{
							name = "Group1Hurry",
							uiType = "Text",
							text = "Pendragon and Aurora are heading to the island.",
							display = function() return Mission.ObjectiveIsSucceeded("Script20") and not Mission.ObjectiveIsSucceeded("Script40") end
						} ,
						UIItem:new
						{
							name = "Group1Missing",
							uiType = "Text",
							text = "Pendragon and Aurora have gone missing!",
							display = function() return Mission.ObjectiveIsSucceeded("Script40") end
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
							function() return not Mission.ObjectiveIsSucceeded("Script10") end

						} ,
						UIItem:new
						{
							name = "Group2Text",
							uiType = "Text",
							text = "Grant Creston and Alec Parson are fighting Warriors.",
							display = function() return not Mission.ObjectiveIsSucceeded("Script10") end
						} ,
						UIItem:new
						{
							name = "Group2Bar",
							uiType = "BarBlue",
							text = "5th Column Arrested:",
							currentVal = function() return status2 end,
							targetVal = function() return 40 end,
							display = function() return not Mission.ObjectiveIsSucceeded("Script10") end
						} ,
						UIItem:new
						{
							name = "Group2Hurry",
							uiType = "Text",
							text = "Grant and Alec are heading to the island.",
							display = function() return Mission.ObjectiveIsSucceeded("Script30") and not Mission.ObjectiveIsSucceeded("Script50") end
						} ,
						UIItem:new
						{
							name = "Group2Missing",
							uiType = "Text",
							text = "Grant and Alec have gone missing!",
							display = function() return Mission.ObjectiveIsSucceeded("Script50") end
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
							function() return not Mission.ObjectiveIsSucceeded("Script10") end
						} ,
						UIItem:new
						{
							name = "Group3Text",
							uiType = "Text",
							text = "Riptide is looking for Praetor White.",
							display = function() return not Mission.ObjectiveIsSucceeded("Script50") end
						} ,
						UIItem:new
						{
							name = "Group3Missing",
							uiType = "Text",
							text = "Riptide has gone missing!",
							display = function() return Mission.ObjectiveIsSucceeded("Script60") end
						} ,
							UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
						} ,
						UIItem:new
						{
							name = "Group4Text",
							uiType = "Text",
							text = "Praetor White has gone missing!",
						} ,
					}
				}
			}
		}
	end
end

-- This says that the following should be refreshed at every tick.
stage1.tick = function(self)
	status1 = countGroupStatus(status1 or 0, 1, "Group1Timer")
	status2 = countGroupStatus(status2 or 0, 1, "Group2Timer")
	status3 = countGroupStatus(status3 or 0, 1, "Group3Timer")

	if not self.ExplodeDone and Mission.ObjectiveIsSucceeded("TriggerVolume_Explode") then
		Entity.PlayFX(player, "WORLD/City/Explosion_FullScreen.fx")
		self.ExplodeDone = true
	end

	if not self.FadeDone and Mission.ObjectiveIsSucceeded("Script58") then
		self.FadeDone = true
		Script.SkyFileFade(Script.SkyFileGetByName("Sky_Praetoria_FirstWard_A.txt"), Script.SkyFileGetByName("i24_Outdoor_Fire.txt"), 1.0)
	end

end
-- Name the Event
SZEMain.eventName = "Brickstown_Arc_2_Mission_3"

-- Add Stages listed above
SZEMain:addStage("startup", stage1)

-- Define stage to start
SZEMain:gotoStage("startup")
