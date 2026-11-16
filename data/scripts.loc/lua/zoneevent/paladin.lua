-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
LoadLuaFile("Lua/Library/Revive.lua")
-- Required to run Scripted Zone Event
tick = function() szeTick() end

-- STAGE DEFINITIONS ##################################################################################################
-- The following stages may consist of: startup, shutdown, tick.
-- Tick will take place as often as scripted zone events are updated, while the stage is active.

-- Setup Stage ----------------------------------------------------------------
setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)
	Message.DebugPrint("Starting Stage Setup for Paladin Zone Event")

	teamNames = {"ShiningStars", "Conduit", "Paladin", "Destructibles", "KnightsErrant"}
	for i, v in ipairs(teamNames) do
		SZEMain:addTeam(v, SZETeam:new{ name = v, layout = v, killBehavior = "DestroyMe"} )
	end

--  Lines 16 to 18 is doing the following in a more efficient manner:
--	SZEMain:addTeam("ShiningStars", SZETeam:new{ name = "ShiningStars", layout = "ShiningStars", killBehavior = "DestroyMe"} )

	SZEMain:addCounter("Conduit", Counter:new{ ctype = "Team", team = "Conduit", target = 12 })
	SZEMain:addCounter("Objects", Counter:new{ ctype = "Team", team = "Destructibles", target = 1000 })
	SZEMain:addCounter("Paladin", Counter:new{ ctype = "Team", team = "Paladin", target = 1 })

	SZEMain:addTimer("Reset", Timer:new{ duration = 20*60 }, false)

	SZEMain:addWaypoint("Conduits", SZEWaypoint:new{ location = "marker:Conduit", text = "Psionic Resonance!", icon = "map_enticon_mission_c" })
	SZEMain:addWaypoint("ShiningStars", SZEWaypoint:new{ location = "marker:ShiningStars", text = "The Shining Stars", icon = "map_enticon_mission_c" })

	self.nextStage = "Stage1"
end
setupStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Startup")
end

function CheckPaladinAI()
	if(AI.CurBehaviorIs(paladin, "AttackTarget")) then
		return
	end

	local nearbyObjects = Entity.GetAllEntities("Objects", nil, nil, nil, nil, 100.0, paladin, nil, true, false, 1, false)
	for i, v in ipairs(nearbyObjects) do
		print("Found: " .. v)
	end

	if(nearbyObjects[1] and nearbyObjects[1] ~= TEAM_NONE) then
		AI.SetAttackTarget(paladin, nearbyObjects[1], 3)
	end
end

stage1 = Stage:new{ name = "Stage1" }
stage1.startup = function(self)
	Message.DebugPrint("Starting Stage 1 for Paladin Zone Event")
	SZEMain.teams["Paladin"]:activate()
	paladin = Team.GetEntityFromTeam("Paladin", 1);
	AI.AddBehavior(paladin, "LimitedSpeed(14),Invincible(1),CombatOverride(Passive),Patrol(NamedRoute(Paladin))")
	SZEMain.teams["Destructibles"]:activate()
	SZEMain.teams["Conduit"]:activate()
	SZEMain.waypoints["Conduits"]:activate()


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
					scriptDisplayName = "The Clockwork Paladin",
					-- StagedisplayName is the text that shows up inside of the actual UI.
					stageDisplayName = "Paladin Rampage!",
					items =
					{
						UIItem:new
						{
							-- Name is the UIItem name from old zone event scripts.
							name = "ObjectDestructionUI",
							uiType = "BarRed",
							text = "Property Damange ($)",
							currentVal = function() return SZEMain.counters["Objects"].progress * 100 end,
							targetVal = function() return 10000 end,
							display = function() return (SZEMain.counters["Objects"].progress * 100) < 10000 end,
							find = "Paladin"
						} ,
						UIItem:new
						{
							-- Name is the UIItem name from old zone event scripts.
							name = "ConduitsUI",
							uiType = "BarRed",
							text = "Psionic Brass Conduits defeated:",
							currentVal = function() return SZEMain.counters["Conduit"].progress end,
							targetVal = function() return 12 end
						} ,
						UIItem:new
						{
							name = "Space0",
							uiType = "CenterText",
							text = "-----",
						} ,
						UIItem:new
						{
							name = "Explain",
							uiType = "CenterText",
							text = "Defeat the Psionic Brass Network",
						} ,
						UIItem:new
						{
							name = "Explain2",
							uiType = "CenterText",
							text = "to make the Paladin vulnerable to attack!",
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "CenterText",
							text = "-----",
						} ,
						UIItem:new
						{
							name = "Achieve",
							uiType = "CenterText",
							text = "(Optional) Prevent the Paladin from dealing",
							display = function() return (SZEMain.counters["Objects"].progress * 100) < 10000 end
						} ,
						UIItem:new
						{
							name = "Achieve2",
							uiType = "CenterText",
							text = "$10,000 worth of property damage!",
							display = function() return (SZEMain.counters["Objects"].progress * 100) < 10000 end
						}
					}
				}
			}
		}
	end
end
stage1.tick = function(self)
	CheckPaladinAI()
	if(SZEMain.counters["Conduit"].progress >= 12) then
		SZEMain.waypoints["Conduits"]:deactivate()
		self.nextStage = "Stage2"
	end
end
stage1.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage 1 for Paladin Zone Event")
end

stage2 = Stage:new{ name = "Stage2" }
stage2.startup = function(self)
	Message.DebugPrint("Starting Stage 2 for Paladin Zone Event")
	AI.AddBehavior(paladin, "Invincible(0)")
	SZEMain.teams["Conduit"]:deactivate()
	SZEMain.teams["Destructibles"]:deactivate()
	SZEMain.teams["ShiningStars"]:activate()
	SZEMain.teams["KnightsErrant"]:activate()
	SZEMain.waypoints["ShiningStars"]:activate()
	AI.AddBehavior(paladin, "Clean,LimitedSpeed(14),CombatOverride(Defensive),MoveToPos(marker:PaladinShowdown),FeelingFear(0),FeelingConfidence(1),OverrideSpawnPos(6.04,-42.50,-2240.26),VulnerableLeashedToSpawnDistance(100)")
	shiningstars = Team.GetEntityFromTeam("ShiningStars", 3);
	StarTimers = {}
	StarName = {}
	for i = 1, Team.NumEntitiesInTeam("ShiningStars") do
		StarTimers[i] = Timer:new{ duration = 5}
		StarName[i] = Team.GetEntityFromTeam("ShiningStars", i)
	end

function CheckKnightsAI()
	AI.SetAttackTarget("KnightsErrant", "ShiningStars", 3)
end

function CheckStarsAI(star)
	if(AI.CurBehaviorIs(star, "AttackTarget")) then
		return
	end

	local nearbyObjects = Entity.GetAllEntities(nil, "Clockwork_Knight_Errant", nil, nil, nil, 100.0, star, nil, true, false, 1, false)

	if(nearbyObjects[1] and nearbyObjects[1] ~= TEAM_NONE) then
		AI.SetAttackTarget(star, nearbyObjects[1], 3)
	else
		AI.AddBehavior("FeelingFear(0),FeelingConfidence(1)")
		AI.SetAttackTarget(star, paladin, 3)
	end
end

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Stage2",
					scriptDisplayName = "The Clockwork Paladin",
					stageDisplayName = "Shining Star Showdown",
					items =
					{
						UIItem:new
						{
							name = "PaladinHealth",
							uiType = "BarRed",
							text = "Paladin's Health",
							currentVal = function() return math.ceil(Entity.GetHealth(paladin) * 100) end,
							targetVal = function() return 100 end,
							find = "Paladin",
						} ,
						UIItem:new
						{
							name = "Explain",
							uiType = "CenterText",
							text = "Defeat the Paladin!",
						} ,
						UIItem:new
						{
							name = "Space",
							uiType = "CenterText",
							text = "",
						} ,
						UIItem:new
						{
							name = "Explain2",
							uiType = "CenterText",
							text = "(Optional) Free the Shining Stars!",
						} ,
					}
				}
			}
		}
	end
end
stage2.tick = function(self)
	if not self.captionDone then
		self.captionDone = true
		SZEMain:foreachPlayerInEvent( function(player) Message.Caption(player, "<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color Black><bgcolor White>Have no fear, citizens of High Park!<t=5 caption x=0.3 y=0.2 d=5><scale 1.75><color Black><bgcolor White>The Shining Stars are here to save the day!")  end )
	end

	for i = 1, Team.NumEntitiesInTeam("ShiningStars") do
		CheckStarsAI(StarName[i])
		ReviveCheck(StarName[i],"Oof...","I've got this!",StarTimers[i])
	end

	if(SZEMain.counters["Paladin"].progress >= 1) then
		self.nextStage = "Complete"
	end
end

stage2.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage 2 for Paladin Event")
	SZEMain.waypoints["ShiningStars"]:deactivate()
	self.captionDone = false
end

completeStage = Stage:new{ name = "Complete" }
completeStage.startup = function(self)
	Message.DebugPrint("Starting Complete Stage for Paladin Zone Event")
	-- SZEMain.teams was actually established at the top when teams were first set up. So the function effects all teams that were defined there.
	for k, v in pairs(SZEMain.teams) do
		v:doBehavior("DestroyMe")
		v:deactivate()
	end

	for k, v in pairs(SZEMain.counters) do
		v:init()
	end

	SZEMain.timers["Reset"]:init()

	if not self.UI then
		self.UI = UICollections:new
		{
			collections =
			{
				UICollection:new
				{
					name = "Complete",
					scriptDisplayName = "The Clockwork Paladin",
					stageDisplayName = "The Paladin will attack soon.",
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
		Message.DebugPrint("Reset timer reached for Paladin Zone Event")
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
SZEMain.eventName = "Paladin"
SZEMain.eventVolume = "KRPaladin"

-- Add Stages listed above
SZEMain:addStage("Setup", setupStage)
SZEMain:addStage("Stage1", stage1)
SZEMain:addStage("Stage2", stage2)
SZEMain:addStage("Complete", completeStage)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("Setup")
