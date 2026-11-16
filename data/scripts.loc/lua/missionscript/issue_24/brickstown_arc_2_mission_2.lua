-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end



-- This is the main function that is being used by the event.
countGroupStatus = function(Status, increase, timerName)
	if SZEMain.timers[timerName]:complete() then
		Status = Status + increase
		SZEMain.timers[timerName]:init()
	end
	return Status
end

stage1 = Stage:new{ name = "Startup" }
stage1.startup = function(self)
	Message.DebugPrint("Starting Stage 1 the St. Martial event")

	SZEMain:addTimer("Group1Timer", Timer:new{ duration = 25 }, true)
	SZEMain:addTimer("Group2Timer", Timer:new{ duration = 15 }, true)
	SZEMain:addTimer("Group3Timer", Timer:new{ duration = 3 }, true)

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
					stageDisplayName = "Team Status",
					items =
					{
						UIItem:new
						{
							name = "Group1Text",
							uiType = "Text",
							text = "Pendragon and Aurora are fighting the Petrovichs.",
						} ,
						UIItem:new
						{
							name = "Group1Bar",
							uiType = "BarBlue",
							text = "Petrovichs arrested:",
							currentVal = function() return status1 end,
							targetVal = function() return 3 end
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
						} ,
						UIItem:new
						{
							name = "Group2Text",
							uiType = "Text",
							text = "Grant Creston and Alec Parson are arresting Crey.",
						} ,
						UIItem:new
						{
							name = "Group2Bar",
							uiType = "BarBlue",
							text = "Crey convicts arrested:",
							currentVal = function() return status2 end,
							targetVal = function() return 10 end
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
						} ,
						UIItem:new
						{
							name = "Group3Text",
							uiType = "Text",
							text = "Riptide is defending Skyway City from the Trolls",
						} ,
						UIItem:new
						{
							name = "Group3Bar",
							uiType = "BarBlue",
							text = "Trolls defeated:",
							currentVal = function() return status3 end,
							targetVal = function() return 50 end,
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
							text = "Marauder is investigating a cure to his condition.",
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

	if not self.StatusChecked and status1 == 2 then
			self.StatusChecked = true
			Message.DebugPrint("Status1 has been seen as equal to 2.")
			Mission.SetObjectiveComplete(1, "ScriptHelpPendragon")
			Message.Caption(ALL_PLAYERS, "<t=8 caption x=0.5 y=0.5 d=8><scale 1.75><color White><bgcolor Silver>This is Pendragon, we need help! The final Petrovich is invincible, we need you to destroy something in that lair!")
			SZEMain.timers["Group1Timer"]:stop()
	elseif not self.StatusAdvanced and Mission.ObjectiveIsSucceeded("GlowieActivated") then
			self.StatusAdvanced = true
			Message.DebugPrint("Glowie activated has been done.")
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color White><bgcolor Silver>Thanks! Aurora and I will finish off the final Petrovich!")
			SZEMain.timers["Group1Timer"]:start()
	end

	if not self.CaptionFinished and status1 == 3 then
			self.CaptionFinished = true
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.2 y=0.3 d=5><scale 1.75><color White><bgcolor Silver>Pendragon reporting in. Aurora and I have taken in all the Petrovichs.")
	end

	if not self.CaptionFinished2 and status2 == 10 then
			self.CaptionFinished2 = true
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color Black><bgcolor RoyalBlue>This is Grant. All of the Crey convicts are accounted for, over.")
	end

	if not self.CaptionFinished3 and status3 == 50 then
			self.CaptionFinished3 = true
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color Black><bgcolor LightSeaGreen>Riptide here, these Trolls were nothing. I've faced baby Destroyers tougher than them.")
	end

end

-- Name the Event
SZEMain.eventName = "Brickstown_Arc_2_Mission_2"

-- Add Stages listed above
SZEMain:addStage("startup", stage1)

-- Define stage to start
SZEMain:gotoStage("startup")
