-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- The set up stage sets the counter to be used in the event to zero and its max to be 100.


-- The main UI stage sets the counter up so that when the player completes an objective, the counter gets points added onto it.

counthealth = function(health, damage)
	--local health = 0
	--local increase = 25
	if Mission.ObjectiveIsSucceeded("Awakened1") then
		health = health - damage
	end
	if Mission.ObjectiveIsSucceeded("Awakened2") then
		health = health - damage
	end
	if Mission.ObjectiveIsSucceeded("Awakened3") then
		health = health - damage
	end
	if Mission.ObjectiveIsSucceeded("Awakened4") then
		health = health - damage
	end
	if Mission.ObjectiveIsSucceeded("Awakened5") then
		health = health - damage
	end
	if Mission.ObjectiveIsSucceeded("Awakened6") then
		health = health - damage
	end
	if Mission.ObjectiveIsSucceeded("Awakened7") then
		health = health - damage
	end
	if Mission.ObjectiveIsSucceeded("Awakened8") then
		health = health - damage
	end
	if Mission.ObjectiveIsSucceeded("Awakened9") then
		health = health - damage
	end
	if Mission.ObjectiveIsSucceeded("Awakened10") then
		health = health - damage
	end
	return health
end

setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)

end

setupStage.tick = function(self)
	if Mission.ObjectiveIsSucceeded("ScriptTalkedTo") then
		self.nextStage = "Main"
	end
end

stage2 = Stage:new{ name = "Main" }
stage2.startup = function(self)
	Message.DebugPrint("Starting Stage 1 the St. Martial event")


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
					scriptDisplayName = "Public health",
					-- StagedisplayName is the text that shows up inside of the actual UI.
					stageDisplayName = "Public health",
					items =
					{
						UIItem:new
						{
							name = "DescriptionText",
							uiType = "Text",
							text = "Defeat Awakened in Sewers to weaken barrier",
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
						} ,
						UIItem:new
						{
							-- Name is the UIItem name from old zone event scripts.
							name = "Public health",
							uiType = "BarRed",
							text = "Psychic Barrier Strength:",
							-- CurrentVal needs a function so that it always knows what it's searching for. The function returns the value of the Longbow counter. The counter called Longbow has a number called progress.
							currentVal = function() return counthealth(100, 25) end,
							-- The reason why the below was done was to keep it simple; it refers to the TARGET var that was set in the Longbow counter. If Longbow counter is changed, this is changed.
							targetVal = function() return 100 end
						}
					}
				}
			}
		}
	end
end

stage2.tick = function(self)

	if not self.StatusChecked and function() return counthealth == 0 end then
			self.StatusChecked = true
			Message.DebugPrint("Max number has been reached")
			Mission.SetObjectiveComplete(1, "ScriptAwakenedDefeated")
			Message.Caption(ALL_PLAYERS, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color White><bgcolor DarkRed>We need a hand here! The final Petrovich is invincible, we need you to destroy something in that lair!")
	end

end
-- Name the Event
SZEMain.eventName = "StMartial_Arc_3_Mission_1"

-- Add Stages listed above
SZEMain:addStage("Setup", setupStage)
SZEMain:addStage("Main", stage2)

-- Define stage to start
SZEMain:gotoStage("Setup")
