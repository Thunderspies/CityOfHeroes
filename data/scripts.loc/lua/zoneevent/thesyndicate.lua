-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Requried to run Scripted Zone Event
tick = function() szeTick() end

-- HELPER FUNCTIONS ###################################################################################################
-- Add helper functions to simplify syntax or pull out logic that may be reused.

-- Create and initialize variables for the enemy spawns.
function makeTeams()
	local ppdFountain = Fountain:new()
	ppdFountain:addTeamSizeCount(1, 1, 3)
	ppdFountain:addTeamSizeCount(2, 2, 3)
	ppdFountain:addTeamSizeCount(4, 3, 4)
	ppdFountain.minRespawnTime = 5
	ppdFountain.maxRespawnTime = 20
	
	local synExtShortFountain = Fountain:new()
	synExtShortFountain:addTeamSizeCount(1, 2, 3)
	synExtShortFountain:addTeamSizeCount(2, 3, 4)
	synExtShortFountain:addTeamSizeCount(3, 4, 6)
	synExtShortFountain:addTeamSizeCount(8, 4, 7)
	synExtShortFountain:addTeamSizeCount(10, 5, 8)
	synExtShortFountain:addTeamSizeCount(12, 7, 12)
	synExtShortFountain:addTeamSizeCount(21, 8, 15)
	synExtShortFountain:addTeamSizeCount(23, 9, 18)
	synExtShortFountain:addTeamSizeCount(38, 10, 20)
	synExtShortFountain.minRespawnTime = 1
	synExtShortFountain.maxRespawnTime = 10
	
	local synExtFountain = Fountain:new()
	synExtFountain:addTeamSizeCount(1, 2, 5)
	synExtFountain:addTeamSizeCount(2, 3, 7)
	synExtFountain:addTeamSizeCount(3, 4, 7)
	synExtFountain:addTeamSizeCount(8, 4, 8)
	synExtFountain:addTeamSizeCount(10, 5, 9)
	synExtFountain:addTeamSizeCount(12, 7, 13)
	synExtFountain:addTeamSizeCount(21, 8, 16)
	synExtFountain:addTeamSizeCount(23, 9, 19)
	synExtFountain:addTeamSizeCount(38, 10, 21)
	synExtFountain.minRespawnTime = 1
	synExtFountain.maxRespawnTime = 10
	
	local synRoofFountain = Fountain:new()
	synRoofFountain:addTeamSizeCount(1, 1, 2)
	synRoofFountain:addTeamSizeCount(2, 2, 3)
	synRoofFountain:addTeamSizeCount(3, 3, 4)
	synRoofFountain:addTeamSizeCount(8, 3, 5)
	synRoofFountain:addTeamSizeCount(10, 4, 6)
	synRoofFountain:addTeamSizeCount(12, 6, 10)
	synRoofFountain:addTeamSizeCount(21, 7, 12)
	synRoofFountain:addTeamSizeCount(23, 9, 15)
	synRoofFountain:addTeamSizeCount(38, 10, 17)
	synRoofFountain.minRespawnTime = 1
	synRoofFountain.maxRespawnTime = 5
	
	local synRoofXtraFountain = Fountain:new()
	synRoofXtraFountain:addTeamSizeCount(1, 1, 3)
	synRoofXtraFountain:addTeamSizeCount(2, 1, 4)
	synRoofXtraFountain:addTeamSizeCount(3, 1, 5)
	synRoofXtraFountain:addTeamSizeCount(8, 2, 6)
	synRoofXtraFountain:addTeamSizeCount(10, 3, 7)
	synRoofXtraFountain:addTeamSizeCount(12, 4, 9)
	synRoofXtraFountain:addTeamSizeCount(21, 5, 10)
	synRoofXtraFountain:addTeamSizeCount(23, 6, 11)
	synRoofXtraFountain:addTeamSizeCount(38, 7, 13)
	synRoofXtraFountain.minRespawnTime = 1
	synRoofXtraFountain.maxRespawnTime = 15
	
	SZEMain:addTeam("PPD", SZETeam:new{ name = "PPD", layout = "STPPDWave",
								spawnBehavior = "IgnoreCombatMods,MoveToPos(\"marker:STPPDExtRunTo\"),Combat",
								killBehavior = "RunIntoDoor", fountain = ppdFountain } )
								
	SZEMain:addTeam("SynExtShort", SZETeam:new{ name = "SynExtShort", layout = "STSyndicateExtShortWave",
								spawnBehavior = "IgnoreCombatMods,MoveToPos(\"marker:STSyndicateExtRunTo\"),Combat",
								killBehavior = "DestroyMe", fountain = synExtShortFountain } )
								
	SZEMain:addTeam("SynExt", SZETeam:new{ name = "SynExt", layout = "STSyndicateExtWave",
								spawnBehavior = "IgnoreCombatMods,MoveToPos(\"marker:STSyndicateExtRunTo\"),Combat",
								killBehavior = "RunIntoDoor", fountain = synExtFountain } )
	
	SZEMain:addTeam("SynInt", SZETeam:new{ name = "SynInt", layout = "STSyndicateIntWave",
								spawnBehavior = "IgnoreCombatMods,Combat",
								killBehavior = "Combat(Timer(Rand(0,30))),RunIntoDoor"} )
								
	SZEMain:addTeam("BombSquad", SZETeam:new{ name = "BombSquad", layout = "STBombSquadWave",
								spawnBehavior = "IgnoreCombatMods,Combat",
								killBehavior = "Combat(Timer(Rand(0,30))),RunIntoDoor"} )
	
	SZEMain:addTeam("BossRoof", SZETeam:new{ name = "BossRoof", layout = "STBossRoofWave", killBehavior = "DestroyMe"} )
	SZEMain:addTeam("PPDvsSynRoof", SZETeam:new{ name = "PPDvsSynRoof", layout = "STPPDvsSynRoofWave", killBehavior = "DestroyMe"} )
	SZEMain:addTeam("JessInt", SZETeam:new{ name = "JessInt", layout = "STJessIntWave", killBehavior = "DestroyMe"} )
	SZEMain:addTeam("JennaInt", SZETeam:new{ name = "JennaInt", layout = "STJennaIntWave", killBehavior = "DestroyMe"} )
	SZEMain:addTeam("JasonInt", SZETeam:new{ name = "JasonInt", layout = "STJasonIntWave", killBehavior = "DestroyMe"} )
	
	SZEMain:addTeam("JessRoof", SZETeam:new{ name = "JessRoof", layout = "STJessRoofWave", killBehavior = "DestroyMe"} )
	SZEMain:addTeam("JennaRoof", SZETeam:new{ name = "JennaRoof", layout = "STJennaRoofWave", killBehavior = "DestroyMe"} )
	SZEMain:addTeam("JasonRoof", SZETeam:new{ name = "JasonRoof", layout = "STJasonRoofWave", killBehavior = "DestroyMe"} )
								
	synIntTeams = {"SynInt", "JessInt", "JennaInt", "JasonInt"}
	roofTeams = {"BossRoof", "SynRoof", "PPDvsSynRoof", "Watcher", "Killer"}
	
	SZEMain:addTeam("SynRoof", SZETeam:new{ name = "SynRoof", layout = "STSyndicateRoofWave",
								spawnBehavior = "IgnoreCombatMods,MoveToPos(\"marker:STBossRoofWave\"),Combat",
								killBehavior = "RunIntoDoor", fountain = synRoofFountain } )
	
	SZEMain:addTeam("SynRoofXtra", SZETeam:new{ name = "SynRoofXtra", layout = "STSyndicateRoofXtraWave",
								spawnBehavior = "IgnoreCombatMods,MoveToPos(\"marker:STBossRoofWave\"),Combat",
								killBehavior = "RunIntoDoor", fountain = synRoofXtraFountain } )
								
	SZEMain:addTeam("Wind", SZETeam:new{ name = "Wind", layout = "STWindRoof", killBehavior = "DestroyMe"} )
	SZEMain:addTeam("Watcher", SZETeam:new{ name = "Watcher", layout = "Watcher", spawnBehavior = "DoNotGoToSleep", killBehavior = "DestroyMe"} )
	SZEMain:addTeam("Killer", SZETeam:new{ name = "Killer", layout = "Killer", spawnBehavior = "DoNotGoToSleep", killBehavior = "DestroyMe"} )
end

function makeGlowies()
	SZEMain:addGlowie("Explosive", GlowieSet:new{ location = "STExplosives", quantity = 3, displayName = "Explosive Cache", model = "OO_All_Bomb_Large_01",
								interactText = "You begin placing the explosives", interruptText = "Something stopped you",
								completeText = "You placed the explosives", barText = "Placing...", interactTime = 4, resetDelay = 3000, vanishOnComplete = true} )
end
-- End HELPER FUNCTIONS ###############################################################################################

-- STAGE DEFINITIONS ##################################################################################################
-- The following stages may consist of: startup, shutdown, tick.
-- Tick will take place as often as scripted zone events are updated, while the stage is active.

-- Setup Stage ----------------------------------------------------------------
setupStage = Stage:new{ name = "Setup" }
SZEMain:addStage("Setup", setupStage)
setupStage.startup = function(self)
	Message.DebugPrint("Starting Stage Setup")
	
	doorList = {"STBottomFloorDoor", "STRooftopDoor"}
	for i, v in ipairs(doorList) do
		SZEDoor.lock(v)
	end
	
	-- Setup teleports and add them them SZEMain.
	SZEMain:addTeleport("Interior", Teleport:new{ volume = "STInteriorPorter", destination = "marker:STInteriorPorter" })
	SZEMain.teleports["Interior"]:activate()
	
	-- Setup waypoints and add them them SZEMain.
	SZEMain:addWaypoint("ExteriorPoint", SZEWaypoint:new{ location = "marker:WP-STSyndicateExtRunTo", text = "The Syndicate", icon = "map_enticon_mission_b" })
	SZEMain:addWaypoint("FrontDoorWayPoint", SZEWaypoint:new{ location = "marker:WP-DeliveryGrenades", text = "Agile Corp", icon = "map_enticon_mission_b" })
	SZEMain:addWaypoint("RooftopWayPoint", SZEWaypoint:new{ location = "marker:WP-MissionExit", text = "Rooftop", icon = "map_enticon_mission_b" })
	
	makeTeams()
	makeGlowies()
	
	SZEMain:addCounter("SynExtShort", Counter:new{ ctype = "VillainGroup", direction = "Up", villainGroup = "Syndicate", target = 10 })
	
	SZEMain:addCounter("SynExt", Counter:new{ ctype = "Team", direction = "Up", team = "SynExt", volumeName = "STExterior", target = 40 })
	SZEMain:addCounter("PPD", Counter:new{ ctype = "Team", direction = "Up", team = "PPD", volumeName = "STExterior", target = 40 })
	
	SZEMain:addCounter("SynInt", Counter:new{ ctype = "Team", direction = "Up", team = "SynInt", volumeName = "STInteriorPorter", target = 55 })
	for i, v in ipairs({"JessInt", "JennaInt", "JasonInt"}) do
		SZEMain:addCounter(v, Counter:new{ ctype = "Team", direction = "Up", team = v, target = 1 })
	end
	
	SZEMain:addCounter("Explosive", Counter:new{ ctype = "Glowie", direction = "Up", glowie = "Explosive", target = 3 })
	SZEMain:addCounter("SynRoof", Counter:new{ ctype = "Team", direction = "Up", team = "SynRoof", target = 25 })
	SZEMain:addCounter("SynRoofXtra", Counter:new{ ctype = "Team", direction = "Up", team = "SynRoofXtra", target = 25 })
	SZEMain:addCounter("BossRoof", Counter:new{ ctype = "Team", direction = "Up", team = "BossRoof", target = 1 })
	
	SZEMain:addTeamDefeatKarmaObjective("SynExt", "SynExt", KarmaObjective:new{karmaValue = 10, karmaLife = 15})
	SZEMain:addTeamDefeatKarmaObjective("SynInt", "SynInt", KarmaObjective:new{karmaValue = 10, karmaLife = 15})
	SZEMain:addGlowieKarmaObjective("Explosive", "Explosive", KarmaObjective:new{karmaValue = 10, karmaLife = 35})
	
	SZEMain:addTeamDefeatKarmaObjective("PPDvsSynRoof", "PPDvsSynRoof", KarmaObjective:new{karmaValue = 10, karmaLife = 25})
	SZEMain:addTeamDefeatKarmaObjective("SynRoof", "SynRoof", KarmaObjective:new{karmaValue = 10, karmaLife = 25})
	SZEMain:addTeamDefeatKarmaObjective("SynRoofXtra", "SynRoofXtra", KarmaObjective:new{karmaValue = 10, karmaLife = 25})
	SZEMain:addTeamDefeatKarmaObjective("BossRoof", "BossRoof", KarmaObjective:new{karmaValue = 30, karmaLife = 35})
	for i, v in ipairs({"JessRoof", "JennaRoof", "JasonRoof"}) do
		SZEMain:addTeamDefeatKarmaObjective(v, v, KarmaObjective:new{karmaValue = 20, karmaLife = 15})
		SZEMain:addCounter(v, Counter:new{ ctype = "Team", direction = "Up", team = v, target = 1 })
	end
	
	SZEMain:addTimer("Explosive", Timer:new{ duration = 10*60 }, false)
	SZEMain:addTimer("Boss", Timer:new{ duration = 10*60 }, false)
	SZEMain:addTimer("Reset", Timer:new{ duration = 60*60 }, false)
	SZEMain:addTimer("ResetQuick", Timer:new{ duration = 60 }, false)

	-- Immediately progress to next stage when setup is done.
	self.nextStage = "DefeatSynExtShort"
end
setupStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Startup")
end

-- DefeatSynExtShort Stage: --------------------------------------------------------------
defeatSynExtShortStage = Stage:new{ name = "DefeatSynExtShort" }
SZEMain:addStage("DefeatSynExtShort", defeatSynExtShortStage)
defeatSynExtShortStage.startup = function(self)
	Message.DebugPrint("Starting Stage DefeatSynExtShort")
	SZEMain.waypoints["ExteriorPoint"]:activate()
	SZEMain.teams["SynExtShort"]:activate()
	
	SZEMain.counters["SynExtShort"]:init()
	
	-- Initialize UI
	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "DefeatSynExtShort",
					scriptDisplayName = "The Syndicate",
					stageDisplayName = "Draw out the Syndicate",
					stageTooltip = "Defeat the guards at the front doors of Megalith Corporation to entice The Syndicate into a trap!",
					items = 
					{
						UIItem:new
						{
							name = "Sep",
							uiType = "CenterText",
							text = " - "
						} 
					} 
				} 
			} 
		}
	end
end
defeatSynExtShortStage.tick = function(self)
	if Var.InstanceGet("ZoneEventMutex") ~= "" then
		Message.DebugPrint("Event Shutting Down Due To Other Event Grabbing The Mutex")
		self.nextStage = "ResetNowAndRecycle"
	elseif SZEMain.counters["SynExtShort"]:complete() then
		SZEMain.teams["SynExtShort"]:deactivate()
		Message.DebugPrint("DefeatSynExtShort done: switching to DefeatSynExt")
		self.nextStage = "DefeatSynExt"
	end
end
defeatSynExtShortStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage DefeatSynExtShort")
end

-- DefeatSynExt Stage: --------------------------------------------------------
defeatSynExtStage = Stage:new{ name = "DefeatSynExt" }
SZEMain:addStage("DefeatSynExt", defeatSynExtStage)
defeatSynExtStage.awardsKarma = true
defeatSynExtStage.startup = function(self)
	Message.DebugPrint("Starting Stage DefeatSynExt")
	SZEMain.waypoints["ExteriorPoint"]:activate()
	SZEMain.teams["SynExt"]:activate()
	SZEMain.teams["PPD"]:activate()
	
	SZEMain.counters["SynExt"]:init()
	SZEMain.counters["PPD"]:init()
	
	-- Initialize UI
	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "DefeatSynExt",
					scriptDisplayName = "The Syndicate",
					stageDisplayName = "Beat Back the Syndicate",
					stageTooltip = "The trap is working, PPD have arrived on the scene to aid you.  Wipe out the gangsters before the PPD are forced to withdraw!",
					items = 
					{
						UIItem:new
						{
							name = "SynExt",
							uiType = "BarBlue",
							text = "Syndicate to defeat:",
							shortText = "Syndicate",
							currentVal = function() return SZEMain.counters["SynExt"].progress end,
							targetVal = function() return SZEMain.counters["SynExt"].target end
						},
						UIItem:new
						{
							name = "PPD",
							uiType = "BarRed",
							text = "PPD Remaining:",
							shortText = "PPD",
							currentVal = function() return SZEMain.counters["PPD"].target - SZEMain.counters["PPD"].progress end,
							targetVal = function() return SZEMain.counters["PPD"].target end
						},
					} 
				} 
			} 
		}
	end
end
defeatSynExtStage.tick = function(self)
	if Var.InstanceGet("ZoneEventMutex") ~= "" then
		Message.DebugPrint("Event Shutting Down Due To Other Event Grabbing The Mutex")
		self.nextStage = "ResetNowAndRecycle"
	elseif SZEMain.counters["SynExt"]:complete() then
		Message.DebugPrint("DefeatSynExt done: switching to SynInt")
		self.nextStage = "SynInt"
	elseif SZEMain.counters["PPD"]:complete() then
		Message.DebugPrint("DefeatSynExt done: switching to SynInt")
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "PPD Cannot Hold The Exterior!", 0xff0000ff, "Attention") end)
		self.nextStage = "Reset"
	end
end
defeatSynExtStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage DefeatSynExt")
	SZEMain.teams["SynExt"]:deactivate()
	SZEMain.teams["PPD"]:deactivate()
end

-- SynInt Stage: ------------------------------------------------------------
synIntStage = Stage:new{ name = "SynInt" }
SZEMain:addStage("SynInt", synIntStage)
synIntStage.awardsKarma = true
synIntStage.startup = function(self)
	Message.DebugPrint("Starting Stage SynInt")
	SZEMain.waypoints["ExteriorPoint"]:deactivate()
	SZEMain.waypoints["FrontDoorWayPoint"]:activate()
	
	SZEMain.glowies["Explosive"]:place()
	SZEMain.glowies["Explosive"]:activate()

	for i, v in ipairs(synIntTeams) do
		SZEMain.teams[v]:activate()
	end
	
	for i, v in ipairs({"SynInt", "JessInt", "JennaInt", "JasonInt", "Explosive"}) do
		SZEMain.counters[v]:init()
	end
	
	SZEDoor.unlock("STBottomFloorDoor")
	SZEMain.teleports["Interior"]:deactivate()
	
	SZEMain.timers["Explosive"]:init()
	
	SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "Megalith Doors Breached!", 0xffff00ff, "Attention") end)
	
	-- Initialize UI
	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "DefeatSynInt",
					scriptDisplayName = "The Syndicate",
					stageDisplayName = "Find Ways to Flush Out the Boss",
					stageTooltip = "There is a Syndicate Boss is hiding somewhere in the building.  To draw him out, you can get his attention somehow.",
					items = 
					{
						UIItem:new
						{
							name = "SynInt",
							uiType = "BarBlue",
							text = "Flush Out The Boss"
						}
					} 
				} 
			} 
		}
	end
end
synIntStage.tick = function(self)
	if Var.InstanceGet("ZoneEventMutex") ~= "" then
		Message.DebugPrint("Event Shutting Down Due To Other Event Grabbing The Mutex")
		self.nextStage = "ResetNowAndRecycle"
		return
	end

	local bossesDefeated = true
	for i, v in ipairs({"Jess", "Jenna", "Jason"}) do
		if SZEMain.counters[v .. "Int"]:complete() then
			bossesDefeated = false
		end
	end
	
	local flushedOut = false
	if bossesDefeated and SZEMain.counters["SynInt"].progress >= 25 then
		Message.DebugPrint("Bosses Defeated")
		flushedOut = true
	elseif SZEMain.counters["SynInt"]:complete() then
		Message.DebugPrint("Minions Defeated")
		flushedOut = true
	elseif SZEMain.counters["Explosive"]:complete() and SZEMain.counters["SynInt"].progress >= 25 then
		Message.DebugPrint("Explosives Defeated")
		SZEMain.teams["BombSquad"]:activate()
		flushedOut = true
	elseif SZEMain.timers["Explosive"]:complete() then
		Message.DebugPrint("Explosives Timer Expired - Moving to Reset")
		SZEMain:foreachPlayerInVolume( function(player) Message.Floater(player, "The Boss Has Escaped!", 0xff0000ff, "Attention") end, "STInteriorPorter")
		self.nextStage = "Reset"
	end
	
	if flushedOut then
		Message.DebugPrint("SynInt finished - Moving to Roof")
		SZEMain:foreachPlayerInVolume( function(player) Message.Floater(player, "The Boss Is On The Roof!", 0xffff00ff, "Attention") end, "STInteriorPorter")
		self.nextStage = "Roof"
	end
end
synIntStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage SynInt")
	SZEMain.glowies["Explosive"]:deactivate()
	SZEMain.glowies["Explosive"]:remove()
end

-- Roof Stage: ---------------------------------------------------
roofStage = Stage:new{ name = "Roof" }
SZEMain:addStage("Roof", roofStage)
roofStage.awardsKarma = true
roofStage.startup = function(self)
	Message.DebugPrint("Starting Stage Roof")
	SZEMain.waypoints["FrontDoorWayPoint"]:deactivate()
	SZEMain.waypoints["RooftopWayPoint"]:activate()
	SZEDoor.unlock("STRooftopDoor")
	
	for i, v in ipairs(roofTeams) do
		SZEMain.teams[v]:activate()
	end
	
	for i, v in ipairs({"Jess", "Jenna", "Jason"}) do
		if not SZEMain.counters[v .. "Int"]:complete() then
			SZEMain.teams[v .. "Roof"]:activate()
		end
	end
	
	for i, v in ipairs({"SynRoof", "SynRoofXtra", "BossRoof", "JessRoof", "JennaRoof", "JasonRoof"}) do
		SZEMain.counters[v]:init()
	end
	
	SZEMain.timers["Boss"]:init()
	
	-- Initialize UI
	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Roof",
					scriptDisplayName = "The Syndicate",
					stageDisplayName = "Defeat The Boss",
					stageTooltip = "You've panicked the Syndicate, the boss is on the roof about to get into a chopper!  Get him before he escapes!",
					items = 
					{
						UIItem:new
						{
							name = "BossRoof",
							uiType = "BarRed",
							text = "Syndicate Boss Health:",
							shortText = "Health",
							currentVal = function() return math.ceil(Entity.GetHealth("BossRoof") * 100) end,
							targetVal = function() return 100 end
						},
						UIItem:new
						{
							name = "RoofGuard",
							uiType = "BarBlue",
							text = "Guards Defeated:",
							shortText = "Guards",
							currentVal = function() return roofCounterSum() end,
							targetVal = function() return 25 end
						},
					} 
				} 
			} 
		}
	end
end

function roofCounterSum()
	local sum = 0
	for i, v in ipairs({"SynRoof", "SynRoofXtra", "JessRoof", "JennaRoof", "JasonRoof"}) do
		sum = sum + SZEMain.counters[v].progress
	end
	return sum
end

roofStage.tick = function(self)
	if not unleashed and (SZEMain.counters["BossRoof"]:complete() or SZEMain.counters["SynRoof"]:complete()) then
		unleashed = true
		Message.DebugPrint("Unleashing More Mobs")
		SZEMain.teams["SynRoofXtra"]:activate()
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "Powerful Winds Approaching!", 0xff0000ff, "Attention") end)
		Delay.delay(10, function() SZEMain.teams["Wind"]:activate() end, true)
	end
	
	if SZEMain.timers["Boss"]:complete() then
		Message.DebugPrint("Roof Failed - Resetting")
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "Failure! The Boss Escaped!", 0xff0000ff, "Attention") end)
		self.nextStage = "Reset"
	elseif SZEMain.counters["BossRoof"]:complete() and roofCounterSum() >= 25 then
		Message.DebugPrint("Roof Success - Rewarding")
		SZEMain:foreachPlayerInEvent( function(player) Message.Floater(player, "Zone Event Success!", 0x00ff00ff, "Attention") end)
		self.nextStage = "Win"
	end
end
roofStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Roof")
	unleashed = false
end
	
-- Win Stage: -----------------------------------------------------------------
winStage = Stage:new{ name = "Win" }
SZEMain:addStage("Win", winStage)
winStage.startup = function(self)
	self.nextStage = "Reset"
end
winStage.shutdown = function(self)
	Message.DebugPrint("Rewarding Syndicate ZE Winners")
	Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Success Awarding Karma Rewards")
	local karmaRewards = 
	{ 
		KarmaReward:new
		{
			percentileNeeded = 0,
			pointsNeeded = 1,
			numStagesRequired = 2,
			rewardTable = "ZE_Syndicate_Base_Low"
		},
		KarmaReward:new
		{
			percentileNeeded = 0.33,
			pointsNeeded = 19,
			numStagesRequired = 3,
			rewardTable = "ZE_Syndicate_Base_Medium"
		},
		KarmaReward:new
		{
			percentileNeeded = 0.66,
			pointsNeeded = 25,
			numStagesRequired = 3,
			rewardTable = "ZE_Syndicate_Base_High"
		}
	}
	local karmaThresholds = { 10, 5, 10 }
	
	Karma.HandleRewards(SZEMain.eventName, false, #karmaThresholds, karmaThresholds, #karmaRewards, karmaRewards)
end

function cleanUp()
	Karma.Reset()
	for k, v in pairs(SZEMain.teams) do
		v:doBehavior("DestroyMe")
		v:deactivate()
	end
	SZEMain:foreachPlayerInVolume( function(player) Message.Floater(player, "Emergency evacuation system activated", 0xff0000ff, "Attention") end, "STInteriorPorter")
	for k, v in pairs(SZEMain.teleports) do
		v:activate()
	end
	for k, v in pairs(SZEMain.doors) do
		SZEDoor.lock(k)
	end
	for k, v in pairs(SZEMain.glowies) do
		v:deactivate()
		v:remove()
	end
	for k, v in pairs(SZEMain.waypoints) do
		v:deactivate()
	end
end

-- Reset Stage: ----------------------------------------------------------------
resetStage = Stage:new{ name = "Reset" }
SZEMain:addStage("Reset", resetStage)
resetStage.startup = function(self)
	Message.DebugPrint("Begin Reset Stage")
	SZEMain.timers["Reset"]:init()
	cleanUp()
	
	-- Initialize UI
	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Reset",
					scriptDisplayName = "The Syndicate",
					stageDisplayName = "The assault is completed!",
					stageTooltip = "The PPD will make another push on the Megalith shortly.",
					items = 
					{
						UIItem:new
						{
							name = "Reset",
							uiType = "Timer",
							text = "Next Event Starts in:",
							shortText = "Next Event",
							currentVal = function() return SZEMain.timers["Reset"]:uiValue() end
						}
					} 
				} 
			} 
		}
	end
end
resetStage.tick = function(self)
	if SZEMain.timers["Reset"]:complete() then
		Message.DebugPrint("Reset Timer Elapsed")
		self.nextStage = "DefeatSynExtShort"
	end
end

-- ResetNowAndRecycle Stage: ----------------------------------------------------------------
resetNowAndRecycleStage = Stage:new{ name = "ResetNowAndRecycle" }
SZEMain:addStage("ResetNowAndRecycle", resetNowAndRecycleStage)
resetNowAndRecycleStage.startup = function(self)
	Message.DebugPrint("Begin ResetNowAndRecycle Stage")
	SZEMain.timers["ResetQuick"]:init()
	cleanUp()
end
resetNowAndRecycleStage.tick = function(self)
	if SZEMain.timers["ResetQuick"]:complete() then
		if Var.InstanceGet("ZoneEventMutex") ~= "" then
			SZEMain.timers["ResetQuick"]:init()
		else
			Message.DebugPrint("Reset Timer Elapsed")
			self.nextStage = "DefeatSynExtShort"
		end
	end
end

-- Stop Stage: ----------------------------------------------------------------
stopStage = Stage:new{ name = "Stop" }
stopStage.startup = function(self)
	-- Final stage, cleanup what's left
	cleanUp()
end
-- End STAGE DEFINITIONS ##############################################################################################

-- Name the Event
SZEMain.eventName = "TheSyndicate"
SZEMain.eventVolume = "SyndicateEventVolume"

-- Define stage to start
SZEMain:gotoStage("Setup")