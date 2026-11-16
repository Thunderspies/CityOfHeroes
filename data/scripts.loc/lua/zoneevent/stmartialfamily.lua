-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- STAGE DEFINITIONS ##################################################################################################
-- The following stages may consist of: startup, shutdown, tick.
-- Tick will take place as often as scripted zone events are updated, while the stage is active.

-- Setup Stage ----------------------------------------------------------------
setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)
	Message.DebugPrint("Starting Stage Setup for Family Zone Event")

	teamNames = {"LongbowStage1", "FamilyStage2", "FamilyObjectsStage2", "LongbowSoldiersStage3", "LongbowObjectsStage3", "LongbowEliteBossStage3"}
	for i, v in ipairs(teamNames) do
		SZEMain:addTeam(v, SZETeam:new{ name = v, layout = v, killBehavior = "DestroyMe"} )
	end

--  Lines 16 to 18 is doing the following in a more efficient manner:
--	SZEMain:addTeam("LongbowStage1", SZETeam:new{ name = "LongbowStage1", layout = "LongbowStage1", killBehavior = "DestroyMe"} )

	SZEMain:addCounter("Longbow", Counter:new{ ctype = "Team", team = "LongbowStage1", target = 15 })
	SZEMain:addCounter("Family", Counter:new{ ctype = "Team", team = "FamilyStage2", target = 20 })
	SZEMain:addCounter("FamilyObject", Counter:new{ ctype = "Team", team = "FamilyObjectsStage2", target = 5 })
	SZEMain:addCounter("LongbowSoldier", Counter:new{ ctype = "Team", team = "LongbowSoldiersStage3", target = 30 })
	SZEMain:addCounter("LongbowObject", Counter:new{ ctype = "Team", team = "LongbowObjectsStage3", target = 5 })
	SZEMain:addCounter("LongbowEliteBoss", Counter:new{ ctype = "Team", team = "LongbowEliteBossStage3", target = 2 })

	self.nextStage = "Stage1"
end
setupStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Startup")
end

stage1 = Stage:new{ name = "Stage1" }
stage1.startup = function(self)
	Message.DebugPrint("Starting Stage 1 for Family Zone Event")
	SZEMain.teams["LongbowStage1"]:activate()
	SZEMain.counters["Longbow"]:init()

--  self.ui is a variable, but hasn't been used before in this script. If it hasn't been used before, then 'if not self.UI then' will be true. If it HAS been defined, then it will be skipped.
--  The reason why is that in the next line we're setting the variable.
--  If not self.UI means, 'If self.UI has not been defined as a variable, then do the next line down', which is setting the self.UI variable.
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
					scriptDisplayName = "The Family Raid",
					-- StagedisplayName is the text that shows up inside of the actual UI.
					stageDisplayName = "Longbow Assault",
					items =
					{
						UIItem:new
						{
							-- Name is the UIItem name from old zone event scripts.
							name = "LongbowUIForStage1",
							uiType = "BarBlue",
							text = "Defeat the Longbow Soldiers",
							-- CurrentVal needs a function so that it always knows what it's searching for. The function returns the value of the Longbow counter. The counter called Longbow has a number called progress.
							currentVal = function() return SZEMain.counters["Longbow"].progress end,
							-- The reason why the below was done was to keep it simple; it refers to the TARGET var that was set in the Longbow counter. If Longbow counter is changed, this is changed.
							targetVal = function() return SZEMain.counters["Longbow"].target end
						}
					}
				}
			}
		}
	end
end
stage1.tick = function(self)
	if not self.captionDone and SZEMain.counters["Longbow"].progress >= 3 then
		self.captionDone = true
		 -- Function(player) is going to pass in something for every player within the event. For each player in the event, we're going to send the Message.Caption to the player.
		SZEMain:foreachPlayerInEvent( function(player) Message.Caption(player, "<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color White><bgcolor DarkRed>This is Commander Bailey. Take down those villains!<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color White><bgcolor DarkRed>The Family has too many illegal weapons in that casino, we must get them back!")  end )
	end

	if SZEMain.counters["Longbow"]:complete() then
		Message.DebugPrint("Stage 1 Complete of Family Zone Event")
		-- "Attention" is a type of floater that you need to specify.
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "Longbow soldiers defeated! Rob the Family!", 0x00ff00ff, "Attention")  end )
		self.nextStage = "Stage2"
	end
end
stage1.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage 1 for Family Zone Event")
	self.captionDone = false
end

stage2 = Stage:new{ name = "Stage2" }
stage2.startup = function(self)
	Message.DebugPrint("Starting Stage 2 for Family Zone Event")
	SZEMain.teams["LongbowStage1"]:deactivate()
	SZEMain.teams["FamilyStage2"]:activate()
	SZEMain.teams["FamilyObjectsStage2"]:activate()
	SZEMain.counters["Family"]:init()
	SZEMain.counters["FamilyObject"]:init()

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Stage2",
					scriptDisplayName = "The Family Raid",
					stageDisplayName = "Family Woes",
					items =
					{
						UIItem:new
						{
							name = "Explain",
							uiType = "Text",
							text = "Enter the building to rob the Family!",
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
						} ,
						UIItem:new
						{
							name = "FamilyKills",
							uiType = "BarBlue",
							text = "Defeat the Family goons",
							currentVal = function() return SZEMain.counters["Family"].progress end,
							targetVal = function() return SZEMain.counters["Family"].target end,
							-- The script below means, "if the SZEMain.counters returns as NOT true, then display this UI."
							display = function() return not SZEMain.counters["Family"]:complete() end
						} ,
						UIItem:new
						{
							name = "FamilyObjectKills",
							uiType = "BarBlue",
							text = "Steal the Family supplies",
							currentVal = function() return SZEMain.counters["FamilyObject"].progress end,
							targetVal = function() return SZEMain.counters["FamilyObject"].target end,
							display = function() return not SZEMain.counters["FamilyObject"]:complete() end
						}
					}
				}
			}
		}
	end
end
stage2.tick = function(self)
	if not self.captionDone and SZEMain.counters["Family"].progress >= 5 then
		self.captionDone = true
		SZEMain:foreachPlayerInEvent( function(player) Message.Caption(player, "<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color Black><bgcolor White>First Longbow, now villains! This is gettin' ridiculous!<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color Black><bgcolor White>This here is OUR turf, villains and Longbow ain't welcome here!")  end )
	end

	if SZEMain.counters["Family"]:complete() and SZEMain.counters["FamilyObject"]:complete() then
		Message.DebugPrint("Stage 2 Complete of Family Zone Event")
		SZEMain:foreachPlayerInEvent( function(player) Message.Caption(player, "<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color White><bgcolor DarkRed>This is Commander Bailey, we have the area surrounded!<t=5 caption x=0.5 y=0.5 d=5><scale 1.75><color White><bgcolor DarkRed>Come out with your hands up!")  end )
		self.nextStage = "Stage3Longbow"
	end
end
stage2.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage 2 for Family Zone Event")
	self.captionDone = false
end

stage3Longbow = Stage:new{ name = "Stage3Longbow" }
stage3Longbow.startup = function(self)
	Message.DebugPrint("Starting Stage 3 Longbow for Family Zone Event")
	SZEMain.teams["FamilyStage2"]:deactivate()
	SZEMain.teams["FamilyObjectsStage2"]:deactivate()
	SZEMain.teams["LongbowSoldiersStage3"]:activate()
	SZEMain.teams["LongbowObjectsStage3"]:activate()
	SZEMain.teams["LongbowEliteBossStage3"]:activate()
	SZEMain.counters["LongbowSoldier"]:init()
	SZEMain.counters["LongbowObject"]:init()
	SZEMain.counters["LongbowEliteBoss"]:init()

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Stage3Longbow",
					scriptDisplayName = "The Family Raid",
					stageDisplayName = "Longbow Showdown",
					items =
					{
						UIItem:new
						{
							name = "LongbowSoldier",
							uiType = "BarBlue",
							text = "Defeat Longbow reinforcements",
							currentVal = function() return SZEMain.counters["LongbowSoldier"].progress end,
							targetVal = function() return SZEMain.counters["LongbowSoldier"].target end,
							display = function() return not SZEMain.counters["LongbowSoldier"]:complete() end
						} ,
						UIItem:new
						{
							name = "LongbowObject",
							uiType = "BarBlue",
							text = "Destroy the Longbow Chasers",
							currentVal = function() return SZEMain.counters["LongbowObject"].progress end,
							targetVal = function() return SZEMain.counters["LongbowObject"].target end,
							display = function() return not SZEMain.counters["LongbowObject"]:complete() end
						} ,
						UIItem:new
						{
							name = "LongbowCommanders",
							uiType = "BarBlue",
							text = "Defeat the Longbow Commanders",
							currentVal = function() return SZEMain.counters["LongbowEliteBoss"].progress end,
							targetVal = function() return SZEMain.counters["LongbowEliteBoss"].target end,
							display = function() return not SZEMain.counters["LongbowEliteBoss"]:complete() end
						}
					}
				}
			}
		}
	end
end
stage3Longbow.tick = function(self)
	if SZEMain.counters["LongbowSoldier"]:complete() and SZEMain.counters["LongbowObject"]:complete() and SZEMain.counters["LongbowEliteBoss"]:complete() then
		Message.DebugPrint("Stage 3 Longbow Complete of Family Zone Event")
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "Longbow has been defeated!", 0x00ff00ff, "Attention")  end )
		-- This TRUE makes sure it's handled for every player in the event. If this was a map-wide zone event, it would try to save time by doing it to all players; but if you do GrantTable, it'll only do it to one player.
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "StMartialZE"); Reward.GrantTable(player, "ZE_SM_Complete")  end, true )
		self.nextStage = "Complete"
	end
end
stage3Longbow.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage 3 Longbow for Family Zone Event")
end

completeStage = Stage:new{ name = "Complete" }
completeStage.startup = function(self)
	Message.DebugPrint("Starting Complete Stage for Family Zone Event")
	-- SZEMain.teams was actually established at the top when teams were first set up. So the function effects all teams that were defined there.
	for k, v in pairs(SZEMain.teams) do
		v:doBehavior("DestroyMe")
		v:deactivate()
	end

	if not SZEMain.timers["Reset"] then
		-- SZEMain:addTimer (name, timer, start), so NAME = "Reset", Timer is the whole thing here with time. TRUE means that it starts once this is all defined.
		SZEMain:addTimer("Reset", Timer:new{ duration = 3*60 }, true)
	else
		SZEMain.timers["Reset"]:init()
	end

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Complete",
					scriptDisplayName = "The Family Raid",
					stageDisplayName = "The Family Raid will begin soon.",
					items =
					{
						UIItem:new
						{
							name = "Timer",
							uiType = "Timer",
							text = "Next Event Starts in:",
							-- You don't need short text.
							shortText = "Next Event:",
							currentVal = function() return SZEMain.timers["Reset"]:uiValue() end
						}
					}
				}
			}
		}
	end
end
completeStage.tick = function(self)
	if SZEMain.timers["Reset"]:complete() then
		Message.DebugPrint("Reset timer reached for Family Zone Event")
		self.nextStage = "Stage1"
	end
end

-- Stop Stage: ----------------------------------------------------------------
stopStage = Stage:new{ name = "Stop" }
stopStage.startup = function(self)
	Message.DebugPrint("Starting Stage Stop")

	for k, v in pairs(SZEMain.teams) do
		v:doBehavior("DestroyMe")
		v:deactivate()
	end

	EndScript()
end
-- End STAGE DEFINITIONS ##############################################################################################

-- Name the Event
SZEMain.eventName = "FamilyRaid"
SZEMain.eventVolume = "EventVolume_Family"

-- Add Stages listed above
SZEMain:addStage("Setup", setupStage)
SZEMain:addStage("Stage1", stage1)
SZEMain:addStage("Stage2", stage2)
SZEMain:addStage("Stage3Longbow", stage3Longbow)
SZEMain:addStage("Complete", completeStage)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("Setup")
