-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- The set up stage sets the counter to be used in the event to zero and its max to be 100.


-- The main UI stage sets the counter up so that when the player completes an objective, the counter gets points added onto it.

countFame = function(fame, increase)
	--local fame = 0
	--local increase = 25
	if Mission.ObjectiveIsSucceeded("ScriptCars15") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("ScriptCars30") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("ScriptCars45") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("ScriptCars60") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("ScriptCars69") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("Family1") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("Family2") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("Family3") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("Family4") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("Family5") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("Family6") then
		fame = fame + increase
	end
	if Mission.ObjectiveIsSucceeded("CrimsonMultiply") then
		fame = fame + 50
	end
	if Mission.ObjectiveIsSucceeded("ScriptSpawnAlly") then
		fame = fame + 50
	end
	return fame
end

stage1 = Stage:new{ name = "Startup" }
stage1.startup = function(self)
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
					scriptDisplayName = "Public Fame",
					-- StagedisplayName is the text that shows up inside of the actual UI.
					stageDisplayName = "Public Fame",
					items =
					{
						UIItem:new
						{
							-- Name is the UIItem name from old zone event scripts.
							name = "Public Fame",
							uiType = "BarRed",
							text = "Gain fame by causing chaos!",
							-- CurrentVal needs a function so that it always knows what it's searching for. The function returns the value of the Longbow counter. The counter called Longbow has a number called progress.
							currentVal = function() return countFame(300, 80) end,
							-- The reason why the below was done was to keep it simple; it refers to the TARGET var that was set in the Longbow counter. If Longbow counter is changed, this is changed.
							targetVal = function() return 1000 end
						}
					}
				}
			}
		}
	end
end
-- Name the Event
SZEMain.eventName = "StMartial_Arc_2_Mission_2"

-- Add Stages listed above
SZEMain:addStage("startup", stage1)

-- Define stage to start
SZEMain:gotoStage("startup")
