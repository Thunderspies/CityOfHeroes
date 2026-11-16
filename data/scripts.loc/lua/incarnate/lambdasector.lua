-- Header file required to use Scripted Zone Events.
LoadLuaFile("Lua/SZE/SZEMain.lua")
-- Requried to run Scripted Zone Event
tick = function() szeTick() end


-- HELPER FUNCTIONS ###################################################################################################
-- Add helper functions to simplify syntax or pull out logic that may be reused.

-- Create and initialize variables for the enemy spawns.
function makeLambdaTeams()
	-- Create fountains and set team sizes (number of players, minimum, maximum).
	local soloFountain = Fountain:new()
	soloFountain:addTeamSizeCount(1, 1, 1)
	
	local guardFountain = Fountain:new()
	guardFountain:addTeamSizeCount(1, 1, 4)
	guardFountain:addTeamSizeCount(15, 1, 5)
	guardFountain:addTeamSizeCount(22, 1, 6)
	
	local reinforceFountainHigh = Fountain:new()
	reinforceFountainHigh:addTeamSizeCount(1, 1, 1)
	reinforceFountainHigh:addTeamSizeCount(13, 2, 2)
	reinforceFountainHigh:addTeamSizeCount(16, 3, 3)
	
	local reinforceFountainMed = Fountain:new()
	reinforceFountainMed:addTeamSizeCount(1, 1, 1)
	reinforceFountainMed:addTeamSizeCount(15, 2, 2)
	reinforceFountainMed:addTeamSizeCount(16, 3, 3)
	
	local exteriorFountain = Fountain:new()
	exteriorFountain:addTeamSizeCount(1, 1, 7)
	exteriorFountain:addTeamSizeCount(13, 1, 8)
	exteriorFountain:addTeamSizeCount(15, 1, 9)
	exteriorFountain:addTeamSizeCount(17, 1, 10)
	exteriorFountain:addTeamSizeCount(22, 1, 11)
	exteriorFountain:addTeamSizeCount(24, 2, 12)
	
	local interiorFountain = Fountain:new()
	interiorFountain:addTeamSizeCount(1, 1, 4)
	interiorFountain:addTeamSizeCount(15, 1, 5)
	interiorFountain:addTeamSizeCount(22, 1, 6)
	
	-- Duplicate interior fountain to patrol fountain.
	local patrolFountain = deepcopy(interiorFountain)
	patrolFountain.minRespawnTime = 60
	
	-- Define tables to store team names.
	defeatStageTeams = {}
	preInteriorStageTeams = {"DestructibleCache", "DestructiblePod", "SecurityTeam-A"}
	doorTeams = {}
	riftTeams = {}
	waveTeams = {}
	
	local basicNames = {"WarmthBuff", "Cutscene", "DestructibleCache", "DestructiblePod", "SecurityTeam-A"}
	
	-- Add incremental letters (A, B, C, ...) to Team names. (example: "ReinforcementDoorA")
	postpendLetters(basicNames, "ReinforcementDoor", 10)
	postpendLetters(doorTeams, "ReinforcementDoor", 10)
	postpendLetters(basicNames, "ReinforcementRift", 10)
	postpendLetters(riftTeams, "ReinforcementRift", 10)
	
	-- Build a counter for each of the door teams and add to SZEMain.
	for i, v in ipairs(doorTeams) do
		local doorCounter = Counter:new
		{
			ctype = "Team",
			direction = "Up",
			team = v,
			target = 1,
		}
		SZEMain:addCounter(v, doorCounter)
	end
	
	-- Add teams to SZEMain.
	for i, v in ipairs(basicNames) do
		SZEMain:addTeam(v, SZETeam:new{ name = v, layout = v, killBehavior = "DestroyMe"} )
	end
	SZEMain:addTeam("Marauder", SZETeam:new{ name = "Marauder", layout = "MarauderTeam", killBehavior = "DestroyMe"} )
	SZEMain:addTeam("LambdaTurrets", SZETeam:new{ name = "LambdaTurrets", layout = "LambdaTurrets",
		spawnBehavior = "DoNotMove,NoPhysics,ProtectSpawnRadius(400),ProtectSpawnOuterRadius(500),ProtectSpawnDuration(30),FOVDegrees(360),OverridePerceptionRadius(500)",
		killBehavior = "DestroyMe"} )
	table.insert(defeatStageTeams, "LambdaTurrets")
	
	SZEMain:addTeam("WeaponCache-Restock", SZETeam:new{ name = "WeaponCache-Restock", layout = "WeaponCache-Restock",
								spawnBehavior = "DoNotGoToSleep", killBehavior = "DestroyMe", fountain = soloFountain } )
							
	SZEMain:addTeam("IncubationPod-Restock", SZETeam:new{ name = "IncubationPod-Restock", layout = "IncubationPod-Restock",
								spawnBehavior = "DoNotGoToSleep", killBehavior = "DestroyMe", fountain = soloFountain } )
	
	SZEMain:addTeam("WeaponCache-Guards", SZETeam:new{ name = "WeaponCache-Guards", layout = "WeaponCache-Guards",
								spawnBehavior = "DoNotGoToSleep", killBehavior = "DestroyMe", fountain = guardFountain } )
	
	SZEMain:addTeam("IncubationPod-Guards", SZETeam:new{ name =  "IncubationPod-Guards", layout =  "IncubationPod-Guards",
								spawnBehavior = "DoNotGoToSleep", killBehavior = "DestroyMe", fountain = guardFountain } )
	
	ReinforcementWave = { name = "ReinforcementWave", layout = "ReinforcementWave",
		spawnBehavior = "DoNotGoToSleep,BoostSpeed(2),Combat", killBehavior = "DestroyMe", fountain = soloFountain,
		onCreate = SZETeam.attachToTeam ("Marauder") }
	
	-- Create Reinforcement waves and add them to SZEMain.
	for i = 1, 12 do
		local Wave = deepcopy(ReinforcementWave)
		Wave.name = postpendLetter(Wave.name, i)
		Wave.layout = Wave.name
		if i == 11 then
			Wave.fountain = reinforceFountainHigh
		elseif i == 12 then
			Wave.fountain = reinforceFountainMed
		end
		SZEMain:addTeam(Wave.name, SZETeam:new(Wave))
		table.insert(waveTeams, Wave.name)
	end
		
	GenIDFExt = {3, 4, 8, 11, 12, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31}
	for i, v in ipairs(GenIDFExt) do
		local Exterior = SZETeam:new{ name = string.format("GenIDF-Ext%02d", v), layout = string.format("GenIDF_Ext%d", v),
			killBehavior = "DestroyMe", fountain = exteriorFountain }
		SZEMain:addTeam(Exterior.name, Exterior)
		table.insert(defeatStageTeams, Exterior.name)
	end
	
	for i=1, 22 do
		local Interior = SZETeam:new{ name = string.format("GenIDF-TF%02d", i), layout = string.format("GenIDF_TF%d", i),
			killBehavior = "DestroyMe", fountain = interiorFountain }
		SZEMain:addTeam(Interior.name, Interior)
		table.insert(preInteriorStageTeams, Interior.name)
	end
	
	for i=1, 17 do
		local Interior = SZETeam:new{ name = string.format("GenIDF-MD%02d", i), layout = string.format("GenIDF_MD%d", i),
			killBehavior = "DestroyMe", fountain = interiorFountain }
		SZEMain:addTeam(Interior.name, Interior)
		table.insert(preInteriorStageTeams, Interior.name)
	end
	
	for i=1, 4 do
		local Interior = SZETeam:new{ name = string.format("GenWW-MD%02d", i), layout = string.format("GenWW_MD%d", i),
			killBehavior = "DestroyMe", fountain = interiorFountain }
		SZEMain:addTeam(Interior.name, Interior)
		table.insert(preInteriorStageTeams, Interior.name)
	end
	
	for i=1, 3 do
		local Patrol = SZETeam:new{ name = string.format("PatrolIDF-TF%02d", i), layout = string.format("PatrolIDF-TF%02d", i),
			spawnBehavior = "UseEnterable,Patrol", killBehavior = "DestroyMe", fountain = patrolFountain }
		SZEMain:addTeam(Patrol.name, Patrol)
		Patrol = SZETeam:new{ name = string.format("PatrolIDF-MD%02d", i), layout = string.format("PatrolIDF-MD%02d", i),
			spawnBehavior = "UseEnterable,Patrol", killBehavior = "DestroyMe", fountain = patrolFountain }
		SZEMain:addTeam(Patrol.name, Patrol)
		table.insert(preInteriorStageTeams, Patrol.name)
	end
end

-- Helper function for Warmth Buff condition.
function checkWarmthOfPrometheus()
	if not Turnstile.IsEventLocked() then
		SZEMain.teams["WarmthBuff"]:activate()
		Message.DebugPrint("Warmth of Prometheus Granted")
		Message.Floater(ALL_PLAYERS, "The Warmth of Prometheus Granted!", 0xff9900ff, "Attention")
	else
		Message.DebugPrint("Warmth of Prometheus Disabled")
	end
end

-- Helper function to calculate how many in this team are phased.
function countPhased(team)
	local n = Team.NumEntitiesInTeam(team)
	local phasedCount = 0
	for i = 1, n do
		local player = Team.GetEntityFromTeam(team, i)
		if Entity.IsPhased(player) then
			phasedCount = phasedCount + 1
		end
	end
	return phasedCount
end

-- Helper function to calculate how many in this team are alive.
function countLiving(team)
	local n = Team.NumEntitiesInTeam(team)
	local livingCount = 0
	for i = 1, n do
		local player = Team.GetEntityFromTeam(team, i)
		if Entity.GetHealth(player) > 0 then
			livingCount = livingCount + 1
		end
	end
	return livingCount
end

-- Helper function to add fail safe timer.
function initFailsafe()
	if not SZEMain.timers["FailsafeShort"] then
		SZEMain:addTimer("FailsafeShort", Timer:new{ duration = 15 }, false)
	else
		SZEMain.timers["FailsafeShort"]:stop()
	end
	
	if not SZEMain.timers["FailsafeLong"] then
		SZEMain:addTimer("FailsafeLong", Timer:new{ duration = 60 }, false)
	else
		SZEMain.timers["FailsafeLong"]:stop()
	end
end

-- Herlper function to monitor Fail Safe.
function checkFailsafe(eventPlayersMin)
	local failsafeTriggered = false
	
	if Script.IsDevMode() then -- no Failsafe at all in dev mode
		return false
	end
	
	local players = countLiving(ALL_PLAYERS) - countPhasedPlayers(ALL_PLAYERS)
	
	if Tunstile.IsEventLocked() then -- need eventPlayerMin - 4 in the zone to continue
		if players < (eventPlayersMin - 4) then
			if not SZEMain.timers["FailsafeLong"]:running() then
				Message.DebugPrint("Starting FailsafeLong Timer")
				SZEMain.timers["FailsafeLong"]:init()
			elseif SZEMain.timers["FailsafeLong"]:complete() then
				Message.DebugPrint("FailsafeLong Timer Complete, Failsafe Triggered")
				failsafeTriggered = true
			end
		else
			Message.DebugPrint("FailsafeLong Canceled")
			SZEMain.timers["FailsafeLong"]:stop()
		end
	else
		if players < 2 then
			if not SZEMain.timers["FailsafeShort"]:running() then
				Message.DebugPrint("Starting FailsafeShort Timer")
				SZEMain.timers["FailsafeShort"]:init()
			elseif SZEMain.timers["FailsafeShort"]:complete() then
				Message.DebugPrint("FailsafeShort Timer Complete, Failsafe Triggered")
				failsafeTriggered = true
			end
		else
			Message.DebugPrint("FailsafeShort Canceled")
			SZEMain.timers["FailsafeShort"]:stop()
		end
	end
	
	return failsafeTriggered
end

-- Helper function to add or initialize hospital Timer
function initHospital()
	if not SZEMain.timers["Hospital"] then
		SZEMain:addTimer("Hospital", Timer:new{ duration = 30 }, true)
	else
		SZEMain.timers["Hospital"]:init()
	end
	
	return { lock = false, note1 = false, note2 = false }
end

-- Helper function to open hospital door.
function unlockHospital(timed)
	SZEDoor.unlock("Door-Hospital")
	if timed then
		SZEMain:foreachPlayerInVolume( function(player) Message.Floater(player, "Hospital Door Unlocked for 10 seconds!", 0x00ff99ff, "PriorityAlert") end, "Hospital")
	else
		SZEMain:foreachPlayerInVolume( function(player) Message.Floater(player, "Hospital Door Unlocked!", 0x00ff99ff, "PriorityAlert") end, "Hospital")
	end
end

-- Helper function to lock hospital door.
function lockHospital()
	SZEDoor.lock("Door-Hospital")
	SZEMain:foreachPlayerInVolume( function(player) Message.Floater(player, "Hospital Door Locked! Next Unlock in 20 seconds!", 0x00ff99ff, "PriorityAlert") end, "Hospital")
end

-- Helper function to verify players in the hospital volume to open the door.
function checkHospital(hospital)
	if not hospital.lock and SZEMain.timers["Hospital"]:value() >= 10 then
		hospital.lock = true
		lockHospital()
	elseif not hospital.note1 and SZEMain.timers["Hospital"]:value() >= 20 then
		hospital.note1 = true
		SZEMain:foreachPlayerInVolume( function(player) Message.Floater(player, "Hospital Door unlocks in 10 seconds!", 0x00ff99ff, "PriorityAlert") end, "Hospital")
	elseif not hospital.note2 and SZEMain.timers["Hospital"]:value() >= 25 then
		hospital.note2 = true
		SZEMain:foreachPlayerInVolume( function(player) Message.Floater(player, "Hospital Door unlocks in 5 seconds!", 0x00ff99ff, "PriorityAlert") end, "Hospital")
	elseif SZEMain.timers["Hospital"]:value() >= 30 then
		hospital.lock, hospital.note1, hospital.note2 = false, false, false
		SZEMain.timers["Hospital"]:init()
		unlockHospital(true)
	end
end

-- Helper function to initialize delivery timers and teams.
function initDelivery(self)
	SZEMain:addTimer("DeliveryStart", Timer:new{ duration = 60 }, true)
	SZEMain:addTimer("Restock", Timer:new{ duration = 120 }, true)
	self.deliveryTeams = { "WeaponCache-Restock", "IncubationPod-Restock", "WeaponCache-Guards", "IncubationPod-Guards" }
end

-- Helper function to add delivery teams to SZEMain.
function checkDelivery(self)
	if not self.deliveryStarted and SZEMain.timers["DeliveryStart"]:complete() then
		self.deliveryStarted = true
		Message.Floater(ALL_PLAYERS, "A Delivery of Weapons Has Arrived from Inside Lambda Sector!", 0xff9900ff, "PriorityAlert")
		Message.Floater(ALL_PLAYERS, "Destroy the Delivery for more Grenades and Acids!", 0xff9900ff, "PriorityAlert")
		for i, v in ipairs(self.deliveryTeams) do
			SZEMain.teams[v]:activate()
		end
		SZEMain.waypoints["DeliveryAcids"]:activate()
		SZEMain.waypoints["DeliveryGrenades"]:activate()
	elseif not self.deliveryCleared and SZEMain.timers["Restock"]:value() > 116 then
		self.deliveryCleared = true
		Message.Floater(ALL_PLAYERS, "The current delivery of weapons is withdrawn!", 0xff9900ff, "PriorityAlert")
		for i, v in ipairs(self.deliveryTeams) do
			SZEMain.teams[v]:doBehavior("Praetorian_TeleportOut(Timer(1),CombatOverride(Passive)),DestroyMe")
		end
	elseif SZEMain.timers["Restock"]:complete() then
		SZEMain.timers["Restock"]:init()
		self.deliveryCleared = false
		Message.Floater(ALL_PLAYERS, "A new delivery of weapons is made!", 0xff9900ff, "PriorityAlert")
		for i, v in ipairs(self.deliveryTeams) do
			SZEMain.teams[v]:forceSpawn()
		end
	end
end

-- Helper function to control Marauder debuff conditions.
function checkMarauderDebuff(self)
	if SZEMain.counters["marauderDebuffed"]:complete() then
		if not SZEMain.timers["Debuff"] then 
			SZEMain:addTimer("Debuff", Timer:new{ duration = 43 }, false)
			marauderEverDebuffed = true
			Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Grenade Use on Marauder Detected")
		end
		
		if not SZEMain.timers["Debuff"]:running() then
			SZEMain.timers["Debuff"]:init()
			Message.Floater(ALL_PLAYERS, "Marauder Pacified!", 0xff9900ff, "PriorityAlert")
		elseif SZEMain.timers["Debuff"]:complete() then
			SZEMain.counters["marauderDebuffed"]:init()
			SZEMain.timers["Debuff"]:stop()
			Message.Floater(ALL_PLAYERS, "Marauder Enrages! Use a Grenade!", 0xff9900ff, "PriorityAlert")
		end
	end
end

-- Helper function to calculate the total number doors that have been killed.
function getDoorKills()
	local doorKills = 0
	for i, v in ipairs(doorTeams) do
		if SZEMain.counters[v]:complete() then
			doorKills = doorKills + 1
		end
	end
	return doorKills
end

-- Helper function to calculate the total number of wave team enemies killed.
function getWaveKills()
	local waveKills = 0
	for i, v in ipairs(waveTeams) do
		waveKills = waveKills + SZEMain.counters[v].progress
	end
	return waveKills
end
-- End HELPER FUNCTIONS ###############################################################################################

-- STAGE DEFINITIONS ##################################################################################################
-- The following stages may consist of: startup, shutdown, tick.
-- Tick will take place as often as scripted zone events are updated, while the stage is active.

-- Setup Stage ----------------------------------------------------------------
setupStage = Stage:new{ name = "Setup" }
setupStage.startup = function(self)
	Message.DebugPrint("Starting Stage Setup")
	Message.DebugPrint("This is an 8-16 player event")
	
	doorList = {"InteriorDoor", "Door-Hospital"}
	for i, v in ipairs(doorList) do
		SZEDoor.lock(v)
	end
	
	interiorDoorList = {"Door-LabElevatorA", "Door-LabElevatorB", "Door-LabElevatorC",
			"Door-WarehouseElevatorA", "Door-WarehouseElevatorB", "Door-WarehouseElevatorC"}
	-- Lock all these doors at start.
	for i, v in ipairs(interiorDoorList) do
		SZEDoor.lock(v)
	end
	
	-- Setup teleports and add them them SZEMain.
	SZEMain:addTeleport("Munitions", Teleport:new{ volume = "MunitionsPorter", destination = "marker:MunitionsPorter" })
	SZEMain.teleports["Munitions"]:activate()
	SZEMain:addTeleport("Training", Teleport:new{ volume = "TrainingPorter", destination = "marker:TrainingPorter" })
	SZEMain.teleports["Training"]:activate()
	
	-- Setup waypoints and add them them SZEMain.
	SZEMain:addWaypoint("DeliveryAcids", SZEWaypoint:new{ location = "marker:WP-DeliveryAcids", text = "Incubation Pod Delivery Point", icon = "map_enticon_neighborhood" })
	SZEMain:addWaypoint("DeliveryGrenades", SZEWaypoint:new{ location = "marker:WP-DeliveryGrenades", text = "Weapon Cache Delivery Point", icon = "map_enticon_neighborhood" })
	SZEMain:addWaypoint("MissionExit", SZEWaypoint:new{ location = "marker:WP-MissionExit", text = "Trial Exit", icon = "map_enticon_neighborhood" })
	SZEMain:addWaypoint("HospitalExit", SZEWaypoint:new{ location = "marker:WP-HospitalExit", text = "Hospital Exit", icon = "map_enticon_neighborhood" })
	
	makeLambdaTeams()

	-- Immediately progress to next stage when setup is done.
	self.nextStage = "DefeatIDF"
end
setupStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Startup")
end

-- Defeat Stage: --------------------------------------------------------------
defeatStage = Stage:new{ name = "DefeatIDF" }
defeatStage.startup = function(self)
	Message.DebugPrint("Starting Stage DefeatIDF")
	SZEMain.waypoints["MissionExit"]:activate()
	SZEMain.waypoints["HospitalExit"]:activate()
	unlockHospital(false)
	for k, v in pairs(defeatStageTeams) do
		SZEMain.teams[v]:activate()
	end
	
	checkWarmthOfPrometheus()
	
	-- Add counter of 60 from IDF to defeat to SZEMain.
	local idfCount = Counter:new
	{
		ctype = "VillainGroup",
		direction = "Up",
		villainGroup = "PraetorianIDFEndgame",
		target = 60,
	}
	SZEMain:addCounter("idfCount", idfCount)
	
	SZEMain:addTimer("Caption", Timer:new{ duration = 5 }, true)
	
	-- Initialize UI
	self.UI = UICollections:new
	{
		collections =
		{
			-- Create a collection entry in the "collections" table.
			UICollection:new
			{
				name = "DefeatStage",
				scriptDisplayName = "Lambda Sector",
				stageDisplayName = "Penetrate Lambda Sector",
				stageTooltip = "Defeat the IDF forces defending Lambda Sector, particularly within its courtyard, to weaken the defenses of the base and gain entry to its interior.",
				items = 
				{
					-- Create UI element to report how many IDF must be defeated.
					UIItem:new
					{
						name = "IDF",
						uiType = "BarBlue",
						text = "IDF to defeat:",
						shortText = "IDF",
						currentVal = function() return SZEMain.counters["idfCount"].progress end,
						targetVal = function() return SZEMain.counters["idfCount"].target end
					} 
				} 
			} 
		} 
	}
end
defeatStage.tick = function(self)
	if not self.captionSaid and SZEMain.timers["Caption"]:complete() then
		self.captionSaid = true
		Message.Caption(ALL_PLAYERS, "<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Now this is something more like I would have chosen for a first target. Lambda Sector, the seat of Emperor Cole's military might and symbol of his tyranny.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Lambda Sector is home to some of the most advanced research and development carried on by Cole's scientists, with secrets developed by both Praetor Berry and the fallen Praetor Keyes.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>In addition, it is the training facility for the Imperial Defense Force, and storehouse to the vast array of weaponry Cole uses to maintain his grip on power.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>From the surface it appears small, but do not be fooled. The surface is but the tip of the iceberg, as the true size of Lambda Sector can only be appreciated from below ground.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Unlike B.A.F., your goal here truly is sabotage. Destroy the weapons within. Defeat his soldiers. Do that, and you will make it so that any response Cole coordinates will not originate from Lambda Sector.")
	end
	
	-- Condition to exit this stage.
	if SZEMain.counters["idfCount"]:complete() then
		Message.Floater(ALL_PLAYERS, "Lambda Sector Facility Breached!", 0xff9900ff, "Attention")
		Message.DebugPrint("DefeatIDF done: switching to PreInterior")
		self.nextStage = "PreInterior"
	end
end
defeatStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage DefeatIDF")
end

-- Pre Interior Stage: --------------------------------------------------------
preInteriorStage = Stage:new{ name = "PreInterior" }
preInteriorStage.startup = function(self)
	Message.DebugPrint("Starting Stage PreInterior")
	unlockHospital(false)
	for k, v in pairs(preInteriorStageTeams) do
		SZEMain.teams[v]:activate()
	end
	SZEDoor.unlock("InteriorDoor")
	SZEMain.teleports["Munitions"]:deactivate()
	SZEMain.teleports["Training"]:deactivate()
	Message.Floater(ALL_PLAYERS, "Defeat Security Guard to Unlock the Facilities!", 0xff9900ff, "Attention")
	SZEMain.timers["Caption"]:init()
	
	local securityTeamACount = Counter:new
	{
		ctype = "Team",
		direction = "Up",
		team = "SecurityTeam-A",
		target = 1,
	}
	SZEMain:addCounter("SecurityTeam-A", securityTeamACount)
	
	self.peopleInside = 0
	self.UI = UICollections:new
	{
		collections = 
		{
			UICollection:new
			{
				name = "PreInteriorStage",
				scriptDisplayName = "Lambda Sector",
				stageDisplayName = "Cripple Lambda Sector",
				stageTooltip = "Lambda Sector's interior is divided into two departments: the Munitions Depot where arms are being stockpiled for a full invasion of Primal Earth and the Training Facility where IDF super soldiers are cloned. Both areas will need to be hit simultaneously if you are to make any progress at slowing their build-up of forces before Praetor White arrives.<br><br>First, however, you will need to eliminate the security guard stationed at the entrance to unlock access to the facilities. After clearing the way, your assault can begin. Any player who uses the doors after defeating the security guard will trigger the next stage.<br><br>While assaulting the inside of the base, keep an eye out for Weapon Caches and Containment Chambers. Weapon Caches will drop Pacification Grenades, which will be of assistance in pacifying Praetor White, allowing you to bypass his defenses more easily. Containment Chambers will drop Molecular Acid, which will allow the reinforcement portals used by the IDF to be destroyed, greatly limiting the number of potential IDF allies Praetor White will call to his aid.<br><br>The Grenades and the Acids will drop randomly to one player present when the object is destroyed. Players can trade these powers and players can also have more than one at a time.",
				items = 
				{
					UIItem:new
					{
						name = "DefeatSecurityTeam",
						uiType = "BarBlue",
						text = "Security Guard Defeated:",
						shortText = "Security Guard",
						currentVal = function() return SZEMain.counters["SecurityTeam-A"].progress end,
						targetVal = function() return SZEMain.counters["SecurityTeam-A"].target end
					},
					UIItem:new
					{
						name = "InteriorFacilityEntry",
						uiType = "BarBlue",
						text = "Interior Facilities Entered:",
						shortText = "Entered Base",
						currentVal = function() return self.peopleInside end, --TODO COUNTERS
						targetVal = function() return 1 end
					} 
				} 
			} 
		} 
	}
end
preInteriorStage.tick = function(self)
	if SZEMain.counters["SecurityTeam-A"]:complete() and not self.securityDefeated then
		self.securityDefeated = true
		Message.Floater(ALL_PLAYERS, "Security Guard Defeated!", 0xff9900ff, "Attention")
		for i, v in ipairs(interiorDoorList) do
			SZEDoor.unlock(v)
		end
		Message.Floater(ALL_PLAYERS, "Facilities Doors Unlocked!", 0xff9900ff, "Attention")
		Message.Floater(ALL_PLAYERS, "Entering Facilities Will Alert Marauder!", 0xff9900ff, "Attention")
		Message.Floater(ALL_PLAYERS, "Do Not Enter Until You Are Prepared!", 0xff9900ff, "Attention")
	end
	
	if not self.captionSaid and SZEMain.timers["Caption"]:complete() then
		self.captionSaid = true
		Message.Caption(ALL_PLAYERS, "<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Excellent. Now, you must enter Lambda Sector. Inside, you'll find a guard. While it is not a major threat, it is keeping the security doors locked. You will not be able to enter until it is defeated.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>This could be a boon, however, as the moment you enter the interior labs, you will trigger an alarm that will call the attention of Emperor Cole. Once you enter, you will have limited time to wreak havoc before Marauder arrives.")
	end
	
	-- Update internal counter variable every tick.
	self.peopleInside = Location.NumEntitiesInArea("trigger:TrainingPorter", ALL_PLAYERS) + Location.NumEntitiesInArea("trigger:MunitionsPorter", ALL_PLAYERS)
	if self.peopleInside > 0 then
		self.nextStage = "Interior"
	end
end
preInteriorStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Pre-Interior")
	SZEMain.teams["SecurityTeam-A"]:deactivate()
end

-- Interior Stage: ------------------------------------------------------------
interiorStage = Stage:new{ name = "Interior" }
interiorStage.startup = function(self)
	initFailsafe()
	self.hospital = initHospital()
	
	Message.DebugPrint("Starting Stage Interior")
	Message.Floater(ALL_PLAYERS, "Praetor White En Route, ETA 5 Minutes!", 0xff9900ff, "Attention")
	Message.Floater(ALL_PLAYERS, "Sabotage the base before his arrival!", 0xff9900ff, "Attention")
	Message.Floater(ALL_PLAYERS, "Recover weapons for the final battle!", 0xff9900ff, "Attention")
	
	SZEMain:addTimer("Interior", Timer:new{ duration = 5*60 }, true)
	SZEMain:addTimer("Acid", Timer:new{ duration = 2 }, false)
	SZEMain:addTimer("Grenade", Timer:new{ duration = 2 }, false)
	SZEMain.timers["Caption"]:init()
	
	-- Stage variable to store current sets killed.
	self.setsKilled = 0
	-- Initialize more team kill counters and add them.
	local cacheKills = Counter:new
	{
		ctype = "Team",
		direction = "Up",
		team = "DestructibleCache",
		target = 10,
	}
	SZEMain:addCounter("cacheKills", cacheKills)
	local cacheKillsSingle = Counter:new
	{
		ctype = "Team",
		direction = "Up",
		team = "DestructibleCache",
		target = 1,
	}
	SZEMain:addCounter("cacheKillsSingle", cacheKillsSingle)
	local podKills = Counter:new
	{
		ctype = "Team",
		direction = "Up",
		team = "DestructiblePod",
		target = 10,
	}
	SZEMain:addCounter("podKills", podKills)
	local podKillsSingle = Counter:new
	{
		ctype = "Team",
		direction = "Up",
		team = "DestructiblePod",
		target = 1,
	}
	SZEMain:addCounter("podKillsSingle", podKillsSingle)
	
	self.UI = UICollections:new
	{
		collections = 
		{
			UICollection:new
			{
				name = "InteriorStage",
				scriptDisplayName = "Lambda Sector",
				stageDisplayName = "Cripple Lambda Sector",
				stageTooltip = "Lambda Sector's interior is divided into two departments: the Munitions Depot where arms are being stockpiled for a full invasion of Primal Earth and the Training Facility where IDF super soldiers are cloned. Both areas will need to be hit simultaneously if you are to make any progress at slowing their build-up of forces before Praetor White arrives.<br><br>First, however, you will need to eliminate the security guard stationed at the entrance to unlock access to the facilities. After clearing the way, your assault can begin. Any player who uses the doors after defeating the security guard will trigger the next stage.<br><br>While assaulting the inside of the base, keep an eye out for Weapon Caches and Containment Chambers. Weapon Caches will drop Pacification Grenades, which will be of assistance in pacifying Praetor White, allowing you to bypass his defenses more easily. Containment Chambers will drop Molecular Acid, which will allow the reinforcement portals used by the IDF to be destroyed, greatly limiting the number of potential IDF allies Praetor White will call to his aid.<br><br>The Grenades and the Acids will drop randomly to one player present when the object is destroyed. Players can trade these powers and players can also have more than one at a time.",
				items =
				{
					UIItem:new
					{
						name = "TimerShow",
						uiType = "Timer",
						text = "Marauder Arrives In:",
						shortText = "Marauder in",
						currentVal = function() return SZEMain.timers["Interior"]:uiValue() end
					},
					UIItem:new
					{
						name = "Caches",
						uiType = "BarBlue",
						text = "Weapon Caches Destroyed:",
						shortText = "Weapons Caches",
						currentVal = function() return SZEMain.counters["cacheKills"].progress end,
						targetVal = function() return SZEMain.counters["cacheKills"].target end
					},
					UIItem:new
					{
						name = "Chambers",
						uiType = "BarBlue",
						text = "Containment Chambers Destroyed:",
						shortText = "Containment Chambers",
						currentVal = function() return SZEMain.counters["podKills"].progress end,
						targetVal = function() return SZEMain.counters["podKills"].target end
					} 
				} 
			} 
		} 
	}
end
interiorStage.tick = function(self)
	if checkFailsafe(8) then
		Message.Floater(ALL_PLAYERS, "Too Few Players to Continue!", 0xff0000ff, "Attention")
		self.nextStage = "Failure"
		return
	end
	checkHospital(self.hospital)
	
	if not self.captionSaid and SZEMain.timers["Caption"]:complete() then
		self.captionSaid = true
		Message.Caption(ALL_PLAYERS, "<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Now that you're inside, you should start looking for Weapon Caches and Containment Chambers. As you tear through Lambda Sector, these items hold some materials you may find valuable in the battles ahead.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Be quick, however. Marauder is responding with the IDF, and you have limited time before he arrives. <t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue> You can delay the arrival by destroying Caches and Chambers, as this will cause the IDF to assess risk to their personnel before deploying.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue> But, ultimately, you will be out of time and the reinforcements, led by Marauder, will arrive. When he does finally arrive, he will re-engage the teleportation systems within the inner rooms.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>These systems teleport anyone not authorized to be in Lambda Sector back to the main entrance of the building. You will have no choice but to confront Marauder.")
	end
	
	if not self.simultaneousAwarded then
		local simultaneous = false
		if SZEMain.counters["cacheKillsSingle"]:complete() then
			SZEMain.counters["cacheKillsSingle"]:init() --reset single counter
			if SZEMain.counters["podKillsSingle"]:complete() or (not SZEMain.timers["Acid"]:complete() and SZEMain.timers["Acid"]:running()) then
				simultaneous = true
			else
				SZEMain.timers["Grenade"]:init()
			end
		elseif SZEMain.counters["podKillsSingle"]:complete() then
			SZEMain.counters["podKillsSingle"]:init() --reset single counter
			if not SZEMain.timers["Grenade"]:complete() and SZEMain.timers["Grenade"]:running() then
				simultaneous = true
			else
				SZEMain.timers["Acid"]:init()
			end
		end
		if simultaneous then
			self.simultaneousAwarded = true
			Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Simultaneous Destroy Badge & Merit Awarded")
			SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "IT_Lambda_Simultaneity"); Reward.GrantTable(player, "AstralMerit1") end, true)
		end
	end
	
	local currentKills = math.min(SZEMain.counters["cacheKills"].progress, SZEMain.counters["podKills"].progress)
	if currentKills > self.setsKilled then -- awarding bonus Time
		local setsToCredit = currentKills - self.setsKilled
		SZEMain.timers["Interior"]:addBonus(60 * setsToCredit)
		Message.Floater(ALL_PLAYERS, "Dual Assault Slows IDF Response", 0x0099ffff, "Attention")
		self.setsKilled = self.setsKilled + setsToCredit
	end
	
	if not self.cachesDestroyed and SZEMain.counters["cacheKills"]:complete() then
		self.cachesDestroyed = true
		Message.Floater(ALL_PLAYERS, "All Weapon Caches Destroyed!", 0xff9900ff, "Attention")
		SZEMain:foreachPlayerInVolume( function(player)	Message.Floater(player, "Munitions Depot Exit Port In 10 Seconds!", 0xff9900ff, "Attention") end, "MunitionsPorter")
		Delay.delay(10, function() SZEMain.teleports["Munitions"]:activate() end, false)
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "All Weapon Caches Merit Awarded")
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "AstralMerit1") end, true)
	end
	if not self.podsDestroyed and SZEMain.counters["podKills"]:complete() then
		self.podsDestroyed = true
		Message.Floater(ALL_PLAYERS, "All Containment Chambers Destroyed!", 0xff9900ff, "Attention")
		SZEMain:foreachPlayerInVolume( function(player)	Message.Floater(player, "Training Facility Exit Port In 10 Seconds!", 0xff9900ff, "Attention") end, "TrainingPorter")
		Delay.delay(10, function() SZEMain.teleports["Training"]:activate() end, false)
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "All Containment Chambers Merit Awarded")
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "AstralMerit1") end, true)
	end
	
	if SZEMain.counters["cacheKills"]:complete() and SZEMain.counters["podKills"]:complete() then
		Message.Floater(ALL_PLAYERS, "Sabotage Complete!", 0xff9900ff, "Attention")
		Message.Floater(ALL_PLAYERS, "Praetor White Arrival Imminent!", 0xff9900ff, "Attention")
		self.nextStage = "Cutscene"
	elseif SZEMain.timers["Interior"]:complete() then
		Message.Floater(ALL_PLAYERS, "No More Time For Sabotage!", 0xff9900ff, "Attention")
		Message.Floater(ALL_PLAYERS, "Praetor White Arrival Imminent!", 0xff9900ff, "Attention")
		self.nextStage = "Cutscene"
	end
end
interiorStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage Interior")
end

-- Marauder Cutscene Stage: ---------------------------------------------------
cutsceneStage = Stage:new{ name = "Cutscene" }
cutsceneStage.startup = function(self)
	SZEMain:addTimer("Cutscene", Timer:new{ duration = 65 }, true)
	unlockHospital(false)
	
	self.UI = UICollections:new
	{
		collections =
		{
			UICollection:new
			{
				name = "CutsceneStage",
				scriptDisplayName = "Lambda Sector",
				stageDisplayName = "Defeat Marauder",
				stageTooltip = "Your sabotage of Lambda Sector has drawn the personal attention of Praetor White, aka Marauder, who is recalling massive IDF forces to repel your incursion. Head to the Lambda Sector courtyard and face him.",
				items = 
				{
					UIItem:new
					{
						name = "Directions",
						uiType = "Text",
						text = "Prepare to confront Marauder"
					},
					UIItem:new
					{
						name = "Cutscene",
						uiType = "Timer",
						text = "Final battle will begin in:",
						shortText = "Final battle",
						currentVal = function() return SZEMain.timers["Cutscene"]:uiValue() end
					}
				} 
			} 
		} 
	}
end
cutsceneStage.tick = function(self)
	if not self.cutscene and SZEMain.timers["Cutscene"]:value() >= 8 then
		self.cutscene = true
		SZEMain.teams["Cutscene"]:activate()
	elseif not self.doors and SZEMain.timers["Cutscene"]:value() >= 18 then
		self.doors = true
		for i, v in ipairs(doorTeams) do
			SZEMain.teams[v]:activate()
		end
	elseif not self.rifts and SZEMain.timers["Cutscene"]:value() >= 20 then
		self.rifts = true
		for i, v in ipairs(riftTeams) do
			SZEMain.teams[v]:activate()
		end
	elseif SZEMain.timers["Cutscene"]:complete() then
		self.nextStage = "Marauder"
	end
end
cutsceneStage.shutdown = function(self)
	-- Close out previous teams and prepare for final battle.
	SZEMain.teams["Cutscene"]:deactivate()
	SZEMain.teams["DestructibleCache"]:deactivate()
	SZEMain.teams["DestructiblePod"]:deactivate()
	SZEDoor.lock("InteriorDoor")
	SZEMain:foreachPlayerInVolume( function(player)	Message.Floater(player, "Munitions Depot Exit Port In 10 Seconds!", 0xff9900ff, "Attention") end, "MunitionsPorter")
	Delay.delay(10, function() SZEMain.teleports["Munitions"]:activate() end, false)
	SZEMain:foreachPlayerInVolume( function(player)	Message.Floater(player, "Training Facility Exit Port In 10 Seconds!", 0xff9900ff, "Attention") end, "TrainingPorter")
	Delay.delay(10, function() SZEMain.teleports["Training"]:activate() end, false)
end

-- Defeat Marauder Stage: -----------------------------------------------------
marauderStage = Stage:new{ name = "Marauder" }
-- Set karma awards for this stage.
marauderStage.awardsKarma = function()
	local players = Team.NumEntitiesInTeam(ALL_PLAYERS)
	local activePlayers = Karma.GetActivePlayers(Script.GetID())
	if players == 0 then
		players = 1
	end
	return activePlayers / players > 0.25
end
marauderStage.startup = function(self)
	initFailsafe()
	self.hospital = initHospital()
	SZEMain:addTimer("Wave", Timer:new{ duration = 30, bonusTime = 30 }, true)
	SZEMain:addTimer("FirstWave", Timer:new{ duration = 60 }, true)
	SZEMain:addTimer("SecondWave", Timer:new{ duration = 60, bonusTime = 30 }, true)
	SZEMain:addTimer("Instructions", Timer:new{ duration = 15 }, true)
	SZEMain:addTimer("Marauder", Timer:new{ duration = 60*20 }, true)
	SZEMain.timers["Caption"]:init()
	SZEMain.teams["Marauder"]:activate()
	for i, v in ipairs(waveTeams) do
		SZEMain.teams[v]:activate()

		local waveCounter = Counter:new
		{
			ctype = "Team",
			direction = "Up",
			team = v,
			target = 100,
		}
		SZEMain:addCounter(v, waveCounter)
	end
	initDelivery(self)
	
	SZEMain:addVillainGroupKarmaObjective("PraetorianIDFEndgame", "IDF", KarmaObjective:new{teamKarmaValue = 10, teamKarmaLife = 20, karmaValue = 10, karmaLife = 20})
	SZEMain:addVillainGroupKarmaObjective("PraetorianWarworksEndgame", "WW", KarmaObjective:new{teamKarmaValue = 10, teamKarmaLife = 20, karmaValue = 10, karmaLife = 20})
	for i, v in ipairs(doorTeams) do
		SZEMain:addTeamDefeatKarmaObjective(v, v, KarmaObjective:new{teamKarmaValue = 20, teamKarmaLife = 60, karmaValue = 30, karmaLife = 60})
	end
	
	local marauderDefeated = Counter:new
	{
		ctype = "Team",
		direction = "Up",
		team = "Marauder",
		target = 1,
	}
	SZEMain:addCounter("marauderDefeated", marauderDefeated)
	
	local marauderOutOfPosition = Counter:new
	{
		ctype = "Name",
		direction = "Up",
		name = "Objects_Lambda_Sector_Watcher",
		target = 1,
	}
	SZEMain:addCounter("marauderOutOfPosition", marauderOutOfPosition)
	
	local marauderDebuffed = Counter:new
	{
		ctype = "Name",
		direction = "Up",
		name = "Objects_Marauder_Debuff_Marker",
		target = 1,
	}
	SZEMain:addCounter("marauderDebuffed", marauderDebuffed)
	
	self.UI = UICollections:new
	{
		collections =
		{
			UICollection:new
			{
				name = "MarauderStage",
				scriptDisplayName = "Lambda Sector",
				stageDisplayName = "Defeat Marauder",
				stageTooltip = "Your sabotage of Lambda Sector has drawn the personal attention of Praetor White, aka Marauder, who is recalling massive IDF forces to repel your incursion. Head to the Lambda Sector courtyard and face him.",
				items = 
				{
					UIItem:new
					{
						name = "Warning",
						uiType = "Text",
						text = "**The Trial fails if Marauder Exits Lambda Sector!**"
					},
					UIItem:new
					{
						name = "Health",
						uiType = "BarRed",
						text = "Marauder Health:",
						shortText = "Marauder",
						find = "Marauder",
						currentVal = function() return math.ceil(Entity.GetHealth("Marauder") * 100) end,
						targetVal = function() return 100 end
					},
					UIItem:new
					{
						name = "Debuff",
						uiType = "BarRed",
						text = "Marauder Enraged:",
						shortText = "Enraged",
						currentVal = function() return SZEMain.counters["marauderDebuffed"].progress end,
						targetVal = function() return SZEMain.counters["marauderDebuffed"].target end
					},
					UIItem:new
					{
						name = "Doors",
						uiType = "BarRed",
						text = "Active Reinforcement Doors:",
						shortText = "Reinforcements",
						currentVal = function() return #doorTeams - getDoorKills() end,
						targetVal = function() return #doorTeams end
					},
					UIItem:new
					{
						name = "Superiority",
						uiType = "BarBlue",
						text = "(Merit Bonus) Reinforcements Defeated:",
						shortText = "(Bonus) IDF: ",
						currentVal = function() return getWaveKills() end,
						targetVal = function() return 30 end,
						display = function() return not self.waveKillsReached end
					},
					UIItem:new
					{
						name = "Restock",
						uiType = "Timer",
						text = "Next Delivery In:",
						shortText = "Delivery",
						currentVal = function() return SZEMain.timers["Restock"]:uiValue() end,
						display = function() return self.deliveryStarted end },
					UIItem:new
					{
						name = "Restock2",
						uiType = "Timer",
						text = "Next Delivery In:",
						shortText = "Delivery",
						currentVal = function() return SZEMain.timers["Restock"]:uiValue() end,
						display = function() return false end
					},
					UIItem:new
					{
						name = "Wave",
						uiType = "Timer",
						text = "Reinforcements In:",
						shortText = "Reinforcements in",
						currentVal = function() return SZEMain.timers["Wave"]:uiValue() end
					},
					UIItem:new
					{
						name = "Remaining",
						uiType = "Timer",
						text = "Time Remaining:",
						shortText = "Remaining",
						currentVal = function() return SZEMain.timers["Marauder"]:uiValue() end
					},
				},
			} 
		} 
	}
end
marauderStage.tick = function(self)
	if checkFailsafe(8) then
		Message.Floater(ALL_PLAYERS, "Too Few Players to Continue!", 0xff0000ff, "Attention")
		self.nextStage = "Failure"
		return
	end
	checkHospital(self.hospital)
	checkDelivery(self)
	
	if not self.captionSaid and SZEMain.timers["Caption"]:complete() then
		self.captionSaid = true
		Message.Caption(ALL_PLAYERS, "<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>This is interesting. I had expected Marauder would be a pushover for you, but that serum he consumed from Praetor Berry has amplified his power several times over. <t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Combined with the reinforcements he is summoning from within Lambda Sector through those gates, and you have your work cut out for you. However, you should have found materials within the base that can help you. <t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>The doors are the simplest. They can be shut down by hurling a Molecular Acid at them. That will be sufficient to put them out of commission and dramatically reduce the number of reinforcements arriving at the base.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Marauder's newfound strength, however, is more problematic. In all our research on the serum he has just consumed, we have found only one weakness: the power it grants can only be maintained for as long as the user is angered.<t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Let me scan you for anything that might help... ah, yes. There. Those pacification grenades will do nicely. <t=8 caption x= 0.2 y=0.2 d=7><scale 1.75><color Paragon><bgcolor MidnightBlue>Those grenades have the power to make even the most out-of-control soldier much calmer. Use them on Marauder and I am certain they will put a stop to his rage temporarily, and with it, his extra powers. Use that window to strike.")
	end
	if not self.instructions and SZEMain.timers["Instructions"]:complete() then
		self.instructions = true
		Message.Floater(ALL_PLAYERS, "Use Grenades to Pacify Marauder, Ending His Enrage!", 0xff9900ff, "Attention")
		Message.Floater(ALL_PLAYERS, "Use Acid to Destroy Portals, Stopping Reinforcements!", 0xff9900ff, "Attention")
	end
	
	-- Loop through each door
	for i = 1, #doorTeams do
		local doorName = doorTeams[i]
		local waveName = waveTeams[i]
		local riftName = riftTeams[i]
		-- Each door counter indicates whether the door has been killed.
		if SZEMain.counters[doorName]:complete() then
			if not self[riftName .. "Closed"] then
				self[riftName .. "Closed"] = true
				SZEMain.teams[riftName]:deactivate()
			end
			if not doorAnyKills then
				-- Log door kill ONCE when the door is killed.
				doorAnyKills = true
				Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Defeat of a Reinforcement Door Detected")
			end
		elseif i % 2 == 1 and SZEMain.timers["FirstWave"]:complete() then
			SZEMain.teams[waveName]:forceSpawn()
		elseif i % 2 == 0 and SZEMain.timers["SecondWave"]:complete() then
			SZEMain.teams[waveName]:forceSpawn()
		end
	end
	if SZEMain.timers["FirstWave"]:complete() then
		SZEMain.teams["ReinforcementWaveK"]:forceSpawn()
		SZEMain.timers["FirstWave"]:init()
	end
	if SZEMain.timers["SecondWave"]:complete() then
		SZEMain.teams["ReinforcementWaveL"]:forceSpawn()
		SZEMain.timers["SecondWave"]:init()
	end
	if SZEMain.timers["Wave"]:complete() then
		SZEMain.timers["Wave"]:init()
	end
	
	checkMarauderDebuff(self)
	
	-- Check if wave kill bonus is achieved.
	if not self.waveKillsReached and getWaveKills() >= 30 then
		self.waveKillsReached = true
		Message.Floater(ALL_PLAYERS, "Superiority over the IDF Demonstrated!", 0xff9900ff, "Attention")
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "30 Reinforcement Wave Merit Awarded")
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "AstralMerit1") end, true)
	end
	
	if SZEMain.counters["marauderDefeated"]:complete() then
		-- Victory condition
		Message.Floater(ALL_PLAYERS, "Marauder Defeated!", 0xff9900ff, "Attention")
		Message.Floater(ALL_PLAYERS, "Lambda Sector Incarnate Trial Succeeded!", 0xff9900ff, "Attention")
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Marauder Defeated Merit Awarded")
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "EmpyreanMerit_Lambda") end, true)
		self.nextStage = "Win"
	elseif SZEMain.counters["marauderOutOfPosition"]:complete() then
		-- Fail condition
		Message.Floater(ALL_PLAYERS, "Marauder Chases You From Lambda Sector!", 0xff9900ff, "Attention")
		Message.Floater(ALL_PLAYERS, "Lambda Sector Incarnate Trial Failed!", 0xff9900ff, "Attention")
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Marauder Out Of Position")
		self.nextStage = "Failure"
	elseif SZEMain.timers["Marauder"]:complete() then
		-- Fail condition
		Message.Floater(ALL_PLAYERS, "Marauder Holds Lambda Sector!", 0xff9900ff, "Attention")
		Message.Floater(ALL_PLAYERS, "Lambda Sector Incarnate Trial Failed!", 0xff9900ff, "Attention")
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Marauder Timer Expired")
		self.nextStage = "Failure"
	end
end
	
-- Win Stage: -----------------------------------------------------------------
winStage = Stage:new{ name = "Win" }
winStage.startup = function(self)
	Message.DebugPrint("Starting Stage RewardWinners")
	Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Awarding Completion Badge")
	SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "IT_Lambda_Complete") end, true)
	
	-- Awards conditions
	if SZEMain.counters["podKills"]:complete() and SZEMain.counters["cacheKills"].progress == 0 and getDoorKills() == 0 then
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Awarding AcidsGatheredNotUsed badge")
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "IT_Lambda_NoAcids") end, true)
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "AstralMerit2") end, true)
	elseif SZEMain.counters["podKills"].progress == 0 and SZEMain.counters["cacheKills"]:complete() and not marauderEverDebuffed then
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Awarding GrenadesGatheredNotUsed badge")
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "IT_Lambda_NoGrenades") end, true)
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "AstralMerit2") end, true)
	elseif SZEMain.counters["podKills"]:complete() and SZEMain.counters["cacheKills"]:complete() and getDoorKills() == 0 and not marauderEverDebuffed then
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Awarding AllGatheredNotUsed badge")
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "IT_Lambda_NoPowers") end, true)
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "AstralMerit1") end, true)
	end
	
	if not Turnstile.IsEventLocked() then
		Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Bonus Merit for an Open League Awarded")
		SZEMain:foreachPlayerInEvent( function(player) Reward.GrantTable(player, "AstralMerit1") end, true)
	end
	
	self.nextStage = "Stop"
end
winStage.shutdown = function(self)
	Message.DebugPrint("Shutting down Stage RewardWinners")
	Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Success Awarding Karma Rewards")
	karmaRewards = 
	{ 
		KarmaReward:new
		{
			numStagesRequired = 0,
			rewardTable = "IncarnatePool1None"
		},
		KarmaReward:new
		{
			pointsNeeded = 10,
			numStagesRequired = 1,
			rewardTable = "Issue20EndKarma1"
		}
	}
	karmaThresholds = { 10 }
	
	Karma.HandleRewards("50plusLambdaSector", false, #karmaThresholds, karmaThresholds, #karmaRewards, karmaRewards)
	-- Set trial status to Success
	Turnstile.CompleteEvent("Success")
end

-- Fail Stage: ----------------------------------------------------------------
failureStage = Stage:new{ name = "Failure" }
failureStage.startup = function(self)
	Message.SZELog(ALL_PLAYERS, SZEMain.eventName, "Failure")
	-- Set trial status to Failure
	Turnstile.CompleteEvent("Failure")
	self.nextStage = "Stop"
end

-- Stop Stage: ----------------------------------------------------------------
stopStage = Stage:new{ name = "Stop" }
stopStage.startup = function(self)
	-- Final stage, cleanup what's left
	for k, v in pairs(SZEMain.teams) do
		v:doBehavior("DestroyMe")
		v:deactivate()
	end
	for k, v in pairs(SZEMain.teleports) do
		v:activate()
	end
	for k, v in pairs(SZEMain.doors) do
		SZEDoor.lock(k)
	end
	unlockHospital(false)
end
-- End STAGE DEFINITIONS ##############################################################################################

-- Name the Event
SZEMain.eventName = "50plusLambdaSector"

-- Add Stages listed above
SZEMain:addStage("Setup", setupStage)
SZEMain:addStage("DefeatIDF", defeatStage)
SZEMain:addStage("PreInterior", preInteriorStage)
SZEMain:addStage("Interior", interiorStage)
SZEMain:addStage("Cutscene", cutsceneStage)
SZEMain:addStage("Marauder", marauderStage)
SZEMain:addStage("Win", winStage)
SZEMain:addStage("Failure", failureStage)
SZEMain:addStage("Stop", stopStage)

-- Define stage to start
SZEMain:gotoStage("Setup")