-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- This event is used in scripts/v_contacts/StMartial/SL6_7_SM_i24_Arc_Three Mission 4.
-- The point of this is to simulate a fight between Penelope Mayhem and Penelope Yin, with Mirror Spirit joining in.

-- This counter is used when you want to have the UI count up something.
countGroupHeal = function(Status, increase, timerName)
	if SZEMain.timers[timerName]:complete() then
		Status = Status + increase
		SZEMain.timers[timerName]:init()
	end
	return Status
end

-- This counter is used when you want to have an enemy health be displayed instead. It's neat!
countGroupDamage = function(Status, damage, timerName)
	if SZEMain.timers[timerName]:complete() then
		Status = Status - damage
		SZEMain.timers[timerName]:init()
	end
	return Status
end

stage1 = Stage:new{ name = "Startup" }
stage1.startup = function(self)
	Message.DebugPrint("Bringing up the UI.")

-- This timer handles Pendragon's damage. He'll get hurt more often thatn Maestro.
	SZEMain:addTimer("GrantCrestonDamageTimer", Timer:new{ duration = 5 }, true)

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
					stageDisplayName = "Riptide's Defense",
					items =
					{
						UIItem:new
						{
							name = "Group1Text",
							uiType = "Text",
							text = "Riptide is holding off Nosferatu!.",
						} ,
						UIItem:new
						{
							name = "GrantCrestonHealth",
							uiType = "BarRed",
							text = "Riptide's Health:",
							currentVal = function() return GrantCrestonHealth end,
							targetVal = function() return 100 end
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
							display = function() return not Mission.ObjectiveIsSucceeded("MainObjective") end

						} ,
						UIItem:new
						{
							name = "Explain",
							uiType = "Text",
							text = "Heal Riptide by activating nearby tech cannisters.",
							display = function() return not Mission.ObjectiveIsSucceeded("MainObjective") end
						} ,
					}
				}
			}
		}
	end
end

-- This says that the following should be refreshed at every tick.
stage1.tick = function(self)

-- This shows the function that's effecting each status.
	GrantCrestonHealth = countGroupDamage(GrantCrestonHealth or 100, 3, "GrantCrestonDamageTimer")



-- The following controls how to heal Pendragon.
	if not self.StatusChecked and Mission.ObjectiveIsSucceeded("Power1") then
			self.StatusChecked = true
			GrantCrestonHealth = GrantCrestonHealth + 15
	end

	if not self.StatusChecked2 and Mission.ObjectiveIsSucceeded("Power2") then
			self.StatusChecked2 = true
			GrantCrestonHealth = GrantCrestonHealth + 15
	end

	if not self.StatusChecked3 and Mission.ObjectiveIsSucceeded("Power3") then
			self.StatusChecked3 = true
			GrantCrestonHealth = GrantCrestonHealth + 15
	end

	if not self.StatusChecked4 and Mission.ObjectiveIsSucceeded("Power4") then
			self.StatusChecked4 = true
			GrantCrestonHealth = GrantCrestonHealth + 15
	end

--  This is an important line. It basically will check to make sure that Pendragon doesn't get something crazy like 150 health and stops displaying.
--  The system will constantly check to see if Pendragon's health value is greater than 100. If it is, then it's going to set it to be 100.
	if GrantCrestonHealth >= 100 then
			GrantCrestonHealth = 100
	end


-- The following handles if Pendragon is defeated.
	if not self.StatusChecked5 and not self.StatusChecked6 and GrantCrestonHealth <= 0 then
			self.StatusChecked5 = true
			Message.DebugPrint("Pendragon is defeated.")
			Mission.SetObjectiveComplete(1, "ScriptGrantCrestonDead")
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color Black><bgcolor LightSeaGreen>Urgh... this is Riptide... I'm retreating...!")
			self.nextStage = "Stop"
	end

	if not self.StatusChecked5 and not self.StatusChecked6 and Mission.ObjectiveIsSucceeded("Chamber") then
			self.StatusChecked6 = true
			Message.DebugPrint("Event was completed before either got to a low state. This means Pendragon is alive..")
			Mission.SetObjectiveComplete(1, "ScriptGrantCrestonAlive")
			Reward.GrantToMissionTeam("Mission_BT_i24_Challenge_04")
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color Black><bgcolor LightSeaGreen>Riptide reporting, I'm bringing Nosferatu to you!")
			self.nextStage = "Stop"
	end

end

-- Stop Stage: ----------------------------------------------------------------
stopStage = Stage:new{ name = "Stop" }
stopStage.startup = function(self)
	Message.DebugPrint("Starting Stage Stop")
	Mission.SetObjectiveComplete(1, "Power1")
	Mission.SetObjectiveComplete(1, "Power2")
	Mission.SetObjectiveComplete(1, "Power3")
	Mission.SetObjectiveComplete(1, "Power4")
	EndScript()
end

-- Name the Event
SZEMain.eventName = "StMartial_Arc_3_Mission_4"

-- Add Stages listed above
SZEMain:addStage("startup", stage1)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("startup")
