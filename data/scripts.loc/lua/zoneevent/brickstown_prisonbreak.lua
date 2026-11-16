LoadLuaFile("Lua/SZE/SZEMain.lua")

tick = function() szeTick() end

setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)
	Message.DebugPrint("Starting Stage Setup for Prison Break Zone Event")

	teamNames = {"PrisonerStage1", "PPDStage1", "PPDVehiclesStage1", "PPDReinforceStage1", "FreakshowStage2", "PPDStage2", "PPDStage3", "PrisonersStage3", "PrisonerBossStage3"}
	for i, v in ipairs(teamNames) do
		SZEMain:addTeam(v, SZETeam:new{ name = v, layout = v, killBehavior = "DestroyMe"} )
	end

	SZEMain:addCounter("Prisoner_S1", Counter:new{ ctype = "Team", team = "PrisonerStage1", target = 20 })
	SZEMain:addCounter("PPD_S1", Counter:new{ ctype = "Team", team = "PPDStage1", target = 10 })
	SZEMain:addCounter("Freakshow_S2", Counter:new{ ctype = "Team", team = "FreakshowStage2", target = 18 })
	SZEMain:addCounter("PrisonerBoss_S3", Counter:new{ ctype = "Team", team = "PrisonerBossStage3", target = 3 })

	self.nextStage = "Stage1"
end
setupStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Startup")
end

stage1 = Stage:new{ name = "Stage1" }
stage1.startup = function(self)
	Message.DebugPrint("Starting Stage 1 for Prison Break Zone Event")
	SZEMain.teams["PrisonerStage1"]:activate()
	SZEMain.teams["PPDStage1"]:activate()
	SZEMain.teams["PPDVehiclesStage1"]:activate()
	SZEMain.counters["Prisoner_S1"]:init()
	SZEMain.counters["PPD_S1"]:init()

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Stage1",
					scriptDisplayName = "Prison Break",
					stageDisplayName = "Help the PPD neutralize the prisoners",
					items =
					{
						UIItem:new
						{
							name = "PrisonerUIForStage1",
							uiType = "BarBlue",
							text = "Prisoners Apprehended",
							currentVal = function() return SZEMain.counters["Prisoner_S1"].progress end,
							targetVal = function() return SZEMain.counters["Prisoner_S1"].target end
						}
					}
				}
			}
		}
	end
end
stage1.tick = function(self)
	if not self.captionDone and SZEMain.counters["PPD_S1"].progress >=  10 then
		self.captionDone = true
		 -- Function(player) is going to pass in something for every player within the event. For each player in the event, we're going to send the Message.Caption to the player.
		SZEMain:foreachPlayerInEvent( function(player) Message.Caption(player, "<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color Gold><bgcolor Black>Sgt. Homme: We're losing too many men! I'm sending in reinforcements.")  end )
		SZEMain.teams["PPDReinforceStage1"]:activate()
	end

	if SZEMain.counters["Prisoner_S1"]:complete() then
		Message.DebugPrint("Stage 1 Complete for Prison Break Zone Event")
		-- "Attention" is a type of floater that you need to specify.
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "Smelling the chaos, Freakshow comes out to play!", 0x00ff00ff, "Attention")  end )
		self.nextStage = "Stage2"
	end
end
stage1.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage 1 for Prison Break Zone Event")
	self.captionDone = false
end

stage2 = Stage:new{ name = "Stage2" }
stage2.startup = function(self)
	Message.DebugPrint("Starting Stage 2 for Prison Break Zone Event")
	SZEMain.teams["FreakshowStage2"]:activate()
	SZEMain.teams["PrisonerStage1"]:deactivate()
	SZEMain.teams["PPDStage2"]:deactivate()
	SZEMain.teams["PPDReinforceStage1"]:doBehavior("MoveToPos(marker:ZE_PrisonerBeacon_Stage3)")
	SZEMain.teams["PPDVehiclesStage1"]:doBehavior("Untargetable(0),Unselectable(0),Invincible(0)")
	SZEMain.counters["Freakshow_S2"]:init()

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Stage2",
					scriptDisplayName = "Prison Break",
					stageDisplayName = "Stop the Freakshow",
					items =
					{
						UIItem:new
						{
							name = "FreakshowKills",
							uiType = "BarBlue",
							text = "Freakshow Subdued",
							currentVal = function() return SZEMain.counters["Freakshow_S2"].progress end,
							targetVal = function() return SZEMain.counters["Freakshow_S2"].target end,
							-- The script below means, "if the SZEMain.counters returns as NOT true, then display this UI."
							display = function() return not SZEMain.counters["Freakshow_S2"]:complete() end
						}
					}
				}
			}
		}
	end
end
stage2.tick = function(self)
	if not self.captionDone and SZEMain.counters["Freakshow_S2"].progress >= 18 then
		self.captionDone = true
		SZEMain:foreachPlayerInEvent( function(player) Message.Caption(player, "<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color Gold><bgcolor Black>Sgt. Homme: Damn! The worst of the inmates have already reached the yard.<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color Gold><bgcolor Black>Sgt. Homme: Looks like it's up to you heroes alone to put those rats back in their cage.")  end )
	end

	if SZEMain.counters["Freakshow_S2"]:complete() then
		Message.DebugPrint("Stage 2 Complete of Prison Break Zone Event")
		-- "Attention" is a type of floater that you need to specify.
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "Superpowered prisoners Have broken free!", 0x00ff00ff, "Attention")  end )
		self.nextStage = "Stage3"
	end
end
stage2.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage 2 for Prison Break Zone Event")
	self.captionDone = false
end

Stage3 = Stage:new{ name = "Stage3" }
Stage3.startup = function(self)
	Message.DebugPrint("Starting Stage 3 Prisoners for Prison Break Zone Event")
	SZEMain.teams["FreakshowStage2"]:deactivate()
	SZEMain.teams["PrisonerStage1"]:deactivate()
	SZEMain.teams["PPDStage1"]:deactivate()
	SZEMain.teams["PPDStage2"]:deactivate()
	SZEMain.teams["PrisonerBossStage3"]:activate()
	SZEMain.teams["PrisonersStage3"]:activate()
	SZEMain.counters["PrisonerBoss_S3"]:init()

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Stage3",
					scriptDisplayName = "Prison Break",
					stageDisplayName = "Defeat the superpowered prisoners",
					items =
					{
						UIItem:new
						{
							name = "PrisonerBoss_S3",
							uiType = "BarBlue",
							text = "Superpowered Prisoners Subdued",
							currentVal = function() return SZEMain.counters["PrisonerBoss_S3"].progress end,
							targetVal = function() return SZEMain.counters["PrisonerBoss_S3"].target end,
							display = function() return not SZEMain.counters["PrisonerBoss_S3"]:complete() end
						}
					}
				}
			}
		}
	end

end
Stage3.tick = function(self)
	if SZEMain.counters["PrisonerBoss_S3"]:complete() then
		Message.DebugPrint("Stage 3 Prisoners Complete of Prison Break Zone Event")
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "Prisoner Bosses have been defeated!", 0x00ff00ff, "Attention")  end )
		-- This TRUE makes sure it's handled for every player in the event. If this was a map-wide zone event, it would try to save time by doing it to all players; but if you do GrantTable, it'll only do it to one player.
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "brickstownze"); Reward.GrantTable(player, "ZE_BT_Complete")  end, true )
		self.nextStage = "Complete"
	end
end
Stage3.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage 3 Prisoners for Prison Break Zone Event")
end

completeStage = Stage:new{ name = "Complete" }
completeStage.startup = function(self)
	Message.DebugPrint("Starting Complete Stage for Prison Break Zone Event")
	-- SZEMain.teams was actually established at the top when teams were first set up. So the function effects all teams that were defined there.
	for k, v in pairs(SZEMain.teams) do
		v:doBehavior("DestroyMe")
		v:deactivate()
	end
	SZEMain.teams["PPDVehiclesStage1"]:activate()
	SZEMain.teams["PPDStage1"]:activate()
	if not SZEMain.timers["Reset"] then
		-- SZEMain:addTimer (name, timer, start), so NAME = "Reset", Timer is the whole thing here with time. TRUE means that it starts once this is all defined.
		SZEMain:addTimer("Reset", Timer:new{ duration = 5*60 }, true)
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
					scriptDisplayName = "Prison Break",
					stageDisplayName = "The Prisoners will breakout soon...",
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
		Message.DebugPrint("Reset timer reached for Prison Break Zone Event")
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
SZEMain.eventName = "PrisonBreak"
SZEMain.eventVolume = "EventVolume_PrisonerBreak"

-- Add Stages listed above
SZEMain:addStage("Setup", setupStage)
SZEMain:addStage("Stage1", stage1)
SZEMain:addStage("Stage2", stage2)
SZEMain:addStage("Stage3", Stage3)
SZEMain:addStage("Complete", completeStage)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("Setup")
