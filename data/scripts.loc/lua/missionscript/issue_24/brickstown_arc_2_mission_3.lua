-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end



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

stage1 = Stage:new{ name = "Startup" }
stage1.startup = function(self)
	Message.DebugPrint("Starting Stage 1 the St. Martial event")

	SZEMain:addTimer("Group1Timer", Timer:new{ duration = 6 }, true)
	SZEMain:addTimer("Group2Timer", Timer:new{ duration = 4 }, true)
--	SZEMain:addTimer("Group3Timer", Timer:new{ duration = 5 }, true)

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
					stageDisplayName = "Sky Raider Showdown",
					items =
					{
						UIItem:new
						{
							name = "Group1Text",
							uiType = "Text",
							text = "The New Praetorians are fighting Colonel Duray.",
						} ,
						UIItem:new
						{
							name = "Group1Bar",
							uiType = "BarRed",
							text = "Colonel Duray's Health:",
							currentVal = function() return status1 end,
							targetVal = function() return 100 end
						} ,
						UIItem:new
						{
							name = "Group2Bar",
							uiType = "BarBlue",
							text = "Sky Raider's Defeated:",
							currentVal = function() return status2 end,
							targetVal = function() return 30 end
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
							text = "Praetor White is looking into a promising lead...",
						} ,
					}
				}
			}
		}
	end
end

-- This says that the following should be refreshed at every tick.
stage1.tick = function(self)
	status1 = countGroupHealth(status1 or 100, 3, "Group1Timer")
	status2 = countGroupStatus(status2 or 0, 1, "Group2Timer")
--	status3 = countGroupStatus(status3 or 0, 1, "Group3Timer")

	if not self.StatusChecked and status1 <= 50 then
			self.StatusChecked = true
			Message.DebugPrint("Status1 has been seen as equal to 2.")
			Mission.SetObjectiveComplete(1, "ScriptHelpPendragon")
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color Brown><bgcolor White>This is Alec Parson, we're being overwhelmed by a remote weapon's system! We need you to disable it on the carrier!")
			SZEMain.timers["Group1Timer"]:stop()
	elseif not self.StatusAdvanced and Mission.ObjectiveIsSucceeded("BossDefeated") then
			self.StatusAdvanced = true
			Message.DebugPrint("Glowie activated has been done.")
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color Brown><bgcolor White>Parson here, we've got it! We'll finish these guys off.")
			SZEMain.timers["Group1Timer"]:start()
	end

	if not self.CaptionFinished and status1 == 0 then
			self.CaptionFinished = true
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color White><bgcolor DarkRed>Parson reporting in, we've got Duray back in custody!")
	end

end
-- Name the Event
SZEMain.eventName = "Brickstown_Arc_2_Mission_3"

-- Add Stages listed above
SZEMain:addStage("startup", stage1)

-- Define stage to start
SZEMain:gotoStage("startup")
