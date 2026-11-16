-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- This script is used for the zone event in the first mission of Brickstown arcs in Arc 2.
-- This zone event is working on conjuction with the mission objectives that are being fired off in the finale mission of this event.
-- The mission is mission 1 in the file data\scripts\Contacts\Brickstown\tasks\_src\SL6_7_Marchand_Arc_Two.xls
-- The goal with this script is that it fires off if the player chooses to handle the side missions in the mission on their own.
-- If the player fails to complete the side missions in the allotted time, floater texts appear to inform the player of failure, then teleport them out of the side mission area.
-- If the player completes the side missions, then they will not be booted out of the area.
-- QUESTIONS: Is it possible to stop a timer? This would make logic easier.

-- STAGE DEFINITIONS ##################################################################################################
-- The following stages may consist of: startup, shutdown, tick.
-- Tick will take place as often as scripted zone events are updated, while the stage is active.

-- Setup Stage ----------------------------------------------------------------

setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)
	Message.DebugPrint("Starting Stage Setup for the Brickstown Event. Waiting for the objective ScriptHandleThem to go off.")

--  TELEPORTER CREATION####################################################################

	SZEMain:addTeleport("TeleportFire", Teleport:new{ volume = "TeleportFire", destination = "marker:TeleportFire" })
	SZEMain:addTeleport("TeleportNemesis", Teleport:new{ volume = "TeleportNemesis", destination = "marker:TeleportNemesis" })
	SZEMain:addTeleport("TeleportFreakshow", Teleport:new{ volume = "TeleportFreakshow", destination = "marker:TeleportFreakshow" })

end

--  ###############################################################################################################################################################

-- The actual event will not start until the mission system succeeds this objective:

setupStage.tick = function(self)
	if Mission.ObjectiveIsSucceeded("ScriptHandleThem") then
		self.nextStage = "TimerUI"
	end
end

--  END OF SET UP STAGE###############################################################################################################################################################


setupStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Startup")
end

--  BEGINNING OF UI TIMER STAGE###############################################################################################################################################################

stage1 = Stage:new{ name = "TimerUI" }
stage1.startup = function(self)
	Message.DebugPrint("Starting Timer UI Stage")

--  TIMER CREATION#######################################################################################################################################################
--  This is setting up the timers to be used in this event.

	if not SZEMain.timers["NemesisTimer"] then
		-- SZEMain:addTimer (name, timer, start), so NAME = "Reset", Timer is the whole thing here with time. TRUE means that it starts once this is all defined.
		SZEMain:addTimer("NemesisTimer", Timer:new{ duration = 5*60 }, true)
	else
		SZEMain.timers["NemesisTimer"]:init()
	end

	if not SZEMain.timers["FreakshowRaveTimer"] then
		-- SZEMain:addTimer (name, timer, start), so NAME = "Reset", Timer is the whole thing here with time. TRUE means that it starts once this is all defined.
		SZEMain:addTimer("FreakshowRaveTimer", Timer:new{ duration = 8*60 }, true)
	else
		SZEMain.timers["FreakshowRaveTimer"]:init()
	end

	if not SZEMain.timers["BuildingFireTimer"] then
		-- SZEMain:addTimer (name, timer, start), so NAME = "Reset", Timer is the whole thing here with time. TRUE means that it starts once this is all defined.
		SZEMain:addTimer("BuildingFireTimer", Timer:new{ duration = 11*60 }, true)
	else
		SZEMain.timers["BuildingFireTimer"]:init()
	end
--  ###############################################################################################################################################################

--  MAIN TIMER UI#######################################################################################################################################################

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "MainTimer",
					scriptDisplayName = "Bring Order to Brickstown",
					stageDisplayName = "Stop the Chaos in Brickstown!",
					items =
					{
						UIItem:new
						{
							name = "NemesisTimer",
							uiType = "Timer",
							text = "Time until Nemesis troops escape:",
							currentVal = function() return SZEMain.timers["NemesisTimer"]:uiValue() end,
							-- The script below means, "Display this if the Nemesis timer isn't complete or NemesisDefeated objective is not complete."
							display = function() return not SZEMain.timers["NemesisTimer"]:complete() and not Mission.ObjectiveIsSucceeded("NemesisDefeated") end
						} ,
						UIItem:new
						{
							name = "NemesisTimerExplanation",
							uiType = "Text",
							text = "Stop Nemesis soldiers from escaping!",
							display = function() return not Mission.ObjectiveIsSucceeded("ScriptNemesisTimer") or Mission.ObjectiveIsSucceeded("NemesisDefeated") end
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
						} ,
						UIItem:new
						{
							name = "FreakshowTimer",
							uiType = "Timer",
							text = "Time until Freakshow go crazy",
							-- You don't need short text.
							currentVal = function() return SZEMain.timers["FreakshowRaveTimer"]:uiValue() end ,
							display = function() return not Mission.ObjectiveIsSucceeded("ScriptFreakshowTimer") or Mission.ObjectiveIsSucceeded("FreakshowDefeated") end
						} ,
						UIItem:new
						{
							name = "FreakshowTimerExplanation",
							uiType = "Text",
							text = "Stop the Freakshow from destroying the warehouse!",
							display = function() return not Mission.ObjectiveIsSucceeded("ScriptFreakshowTimer") or Mission.ObjectiveIsSucceeded("FreakshowDefeated") end
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "Text",
							text = "",
						} ,
						UIItem:new
						{
							name = "BuildingFireTimer",
							uiType = "Timer",
							text = "Time until fire engulfs the building",
							currentVal = function() return SZEMain.timers["BuildingFireTimer"]:uiValue() end ,
							display = function() return not Mission.ObjectiveIsSucceeded("ScriptFireTimer") or Mission.ObjectiveIsSucceeded("PeopleRescued") end
						} ,
						UIItem:new
						{
							name = "BuildingFireExplanation",
							uiType = "Text",
							text = "Rescue civilians from the burning building!",
							display = function() return not Mission.ObjectiveIsSucceeded("ScriptFireTimer") or Mission.ObjectiveIsSucceeded("PeopleRescued") end
						}
					}
				}
			}
		}
	end
end

--  ###############################################################################################################################################################

--  FLOATER FAIL TEXT HANDLE###############################################################################################################################################################
--  If the timer runs out and the mission objective for completing the side missions hasn't been completed, then this floater appears and also kicks the player out of the volume.

stage1.tick = function(self)
	if not self.NemesisFloaterDone and SZEMain.timers["NemesisTimer"]:complete() and not Mission.ObjectiveIsSucceeded("NemesisDefeated") then
		self.NemesisFloaterDone = true
		Message.DebugPrint("Nemesis portion failed")
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "The Nemesis Soldiers escape!", 0x00ff00ff, "Attention")  end )
		SZEMain.teleports["TeleportNemesis"]:activate()
		Mission.SetObjectiveComplete(1, "ScriptNemesisTimer")
	end

	if not self.FreakshowFloaterDone and SZEMain.timers["FreakshowRaveTimer"]:complete() and not Mission.ObjectiveIsSucceeded("FreakshowDefeated") then
		self.FreakshowFloaterDone = true
		Message.DebugPrint("FreakshowPortionFailed")
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "The Freakshow destroy the warehouse!", 0x00ff00ff, "Attention")  end )
		SZEMain.teleports["TeleportFreakshow"]:activate()
		Mission.SetObjectiveComplete(1, "ScriptFreakshowTimer")

	end
	if not self.FireFloaterDone and SZEMain.timers["BuildingFireTimer"]:complete() and not Mission.ObjectiveIsSucceeded("PeopleRescued") then
		self.FireFloaterDone = true
		Message.DebugPrint("Fire Portion Failed")
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "The fire engulfs the building!", 0x00ff00ff, "Attention")  end )
		SZEMain.teleports["TeleportFire"]:activate()
		Mission.SetObjectiveComplete(1, "ScriptFireTimer")

	end
end

--  ###############################################################################################################################################################

-- Stop Stage: ----------------------------------------------------------------
stopStage = Stage:new{ name = "Stop" }
stopStage.startup = function(self)
	Message.DebugPrint("Starting Stage Stop")

	EndScript()
end
-- End STAGE DEFINITIONS ##############################################################################################

-- Name the Event
SZEMain.eventName = "i24_Brickstown_Arc2_Mission1_Script"

-- Add Stages listed above
SZEMain:addStage("Setup", setupStage)
SZEMain:addStage("TimerUI", stage1)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("Setup")
