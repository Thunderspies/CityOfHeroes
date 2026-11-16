SZEMain = {
	updateTicks = 0,
	eventName = nil,
	eventVolume = nil,
	doors = {},
	glowies = {},
	timers = {},
	karmaObjectives = { T = {}, VD = {}, VG = {}, G = {} },
	stages = {},
	currentStage = nil,
	teams = {},
	activeFountainTeams = {},
	teleports = {},			-- indexed by reference name
	teleportsByVolume = {},	-- indexed by volume name, subindexed by reference name
	delayedActions = {},		-- array
	counters = {},
	waypoints = {},			-- indexed by reference name
	activeWaypoints = {}, 		-- indexed by script ID
	entityPetTeams = {},		-- k: entity, v: script team to put their pets on
	currentBGM = {},			-- sound and volume level of current background music to play when people enter event (loop)
	gurneys = nil			-- gurney override manager
}

function SZEMain:update ()
	if self.currentStage then
		local nextStage = self.currentStage.nextStage
		if nextStage then
			self.currentStage.nextStage = nil
			self:gotoStage(nextStage)
		end
	end

	self:checkActiveFountains()
	Delay.processDelayedActions(self.delayedActions)
	
	for k, v in pairs(self.glowies) do	-- update Glowies
		v:update()
	end

	if self.currentStage then
		self.currentStage:update()
	end
	
	if self.updateTicks >= 4 then		-- This stuff doesn't need to be updated every tick
		if self.currentStage and self.currentStage.UI then
			self.currentStage.UI:change()
		end
		DelayedUIRemover:update()
		for k, v in pairs(self.activeWaypoints) do -- update Waypoints
			v:update()
		end
		self.updateTicks = 0
	else
		self.updateTicks = self.updateTicks + 1
	end
end

function SZEMain:playerCount(useActivePlayers)
	local players
	
	if not self.eventVolume then
		players = Team.NumEntitiesInTeamEvenDead(ALL_PLAYERS)
	else
		players = Location.NumEntitiesInArea("trigger:" .. self.eventVolume, ALL_PLAYERS)
	end
	
	if not useActivePlayers then
		return players
	end
	
	-- TODO this is very inaccurate on the startup of a turnstile/missions script. One possible improvement is to get a number from the Turnstile of "players to expect" and spawn with that in mind if there are 0 players in zone
	activePlayers = Karma.GetActivePlayers(Script.GetID())
	if activePlayers <= 0 then
		return math.max(1, players) -- we just started, and noone's made a move, just count everyone as active
	else
		return math.max(math.max(players / 2, activePlayers), 1) -- min of at least 1/2 the people in the zone, zero is special, make sure to be at least 1
	end
end

function SZEMain:checkActiveFountains()
	if not self.activeFountainTeams then
		return
	end
	
	local activePlayers = SZEMain:playerCount(true)
	local allPlayers = SZEMain:playerCount(false)
	
	for k, v in pairs(self.activeFountainTeams) do
		players = v.useActivePlayers and activePlayers or allPlayers
		v.fountain:checkRespawnPop(Team.NumEntitiesInTeam(v.name), players)
		if v.fountain:checkRespawnTimer() then
			v:fountainPopulate(players)
		end
	end
end

function SZEMain:addGlowie (name, glowie)
	if name and glowie and not self.glowies[name] then
		self.glowies[name] = glowie
		glowie:init()
	elseif name and glowie and self.glowies[name] then
		print("Glowie " .. name .. " is already defined in this SZE")
	end
end

function SZEMain:addTeam (name, team)
	if name and team and not self.teams[name] then
		self.teams[name] = team
	elseif name and team and self.teams[name] then
		print("Team " .. name .. " is already defined in this SZE")
	end
end

function SZEMain:addStage (name, stage)
	if name and stage and not self.stages[name] then
		self.stages[name] = stage
	elseif name and stage and self.stage[name] then
		print("Stage " .. name .. " is already defined in this SZE")
	end
end

function SZEMain:addTimer (name, timer, start)
	if name and timer and not self.timers[name] then
		self.timers[name] = timer
		if start then timer:start() end
	elseif name and timer and self.timers[name] then
		print("Timer " .. name .. " is already defined in this SZE")
	end
end

function SZEMain:addTeleport (name, teleport)
	if name and teleport and not self.teleports[name] then
		if not self.teleportsByVolume[teleport.volume] then
			self.teleportsByVolume[teleport.volume] = {}
		end
		
		self.teleports[name] = teleport
		self.teleportsByVolume[teleport.volume][name] = teleport
	elseif name and teleport then
		print("Teleport " .. name .. " is already defined in this SZE")
	end
end

function SZEMain:addWaypoint (name, waypoint)
	if name and waypoint and not self.waypoints[name] then
		self.waypoints[name] = waypoint
	elseif name then
		print("Waypoint " .. name .. " is already defined in this SZE")
	end
end

function SZEMain:addCounter (name, counter)
	if name and counter and not self.counters[name] then
		self.counters[name] = counter
		counter:init()
	elseif name and counter and self.counters[name] then
		print("Counter " .. name .. " is already defined in this SZE")
	end
end

function SZEMain:addGurney (name, gurney)
	self.gurneys:add(name, gurney)
end

function SZEMain:setBGM (sound, volume) -- use nil to clear
	self.currentBGM.sound = sound
	self.currentBGM.volume = volume
	
	if sound then
		SZEMain:foreachPlayerInEvent( function(player) Message.Sound(player, sound, "Music", volume) end, false)
	else
		SZEMain:foreachPlayerInEvent( function(player) Message.ResetMusic(player) end, false)
	end
end

function SZEMain:foreachPlayerInEvent (func, force) -- set force to true if the function doesn not accept TEAMs as it will disable the optimization for map-wide events
	if not self.eventVolume and not force then
		func(ALL_PLAYERS_READYORNOT)
	else
		self:foreachPlayerInVolume(func, self.eventVolume)
	end
end

function SZEMain:foreachPlayerInVolume (func, volume)
	local i, n
	n = Team.NumEntitiesInTeam(ALL_PLAYERS_READYORNOT)
	for i=1,n do
		player = Team.GetEntityFromTeam(ALL_PLAYERS_READYORNOT, i)
		if not volume or Location.EntityInArea(player, "trigger:" .. volume) then
			func(player)
		end
	end
end

function SZEMain:transition (newStage)
	if self.currentStage then
		self.currentStage:shutdown()
	end
	Delay.clearDelayedActionsOnTransition(self.delayedActions)
	
	if self.currentStage and self.currentStage.awardsKarma then
		Karma.AdvanceStage()
	end
	
	self.currentStage = newStage
	
	Var.Set(STATE_VAR, self.currentStage.name);
	Var.SetNumber(STATE_TIME, Script.CurrentTime())
	
	if self.currentStage.awardsKarma and not Karma.IsEnabled() then
		Karma.Enable()
	elseif not self.currentStage.awardsKarma then
		Karma.Disable()
	end
	
	self.currentStage:startup()
	
	if self.currentStage.UI then
		self.currentStage.UI:init()
	else
		SZEMain:foreachPlayerInEvent( function(player) UICollection.hideCurrentUI(player) end, true )
	end
	
	SZEMain:foreachPlayerInEvent( function(player) szeOnEnterMap(player) end )
end

function SZEMain:gotoStage(name)
	if name and SZEMain.stages[name] then
		SZEMain:transition(SZEMain.stages[name])
	elseif name then
		print("Stage " .. name .. " not found in this event.")
	end
end

szeOnEnterMap = function(player)
	if not SZEMain.eventVolume or Location.EntityInArea(player, "trigger:" .. SZEMain.eventVolume) then
		szeOnEnterEvent(player)
	end
end

szeOnEnterEvent = function(player)
	DelayedUIRemover:remove(player)
	if SZEMain.currentBGM.sound then
		Message.Sound(player, SZEMain.currentBGM.sound, "Music", SZEMain.currentBGM.volume)
	end
	if SZEMain.currentStage then
		if SZEMain.currentStage.UI then
			SZEMain.currentStage.UI:showPlayer(player)
		end
		if SZEMain.currentStage.awardsKarma then
			Karma.SetContainer(player)
		end
		for k, v in pairs(SZEMain.activeWaypoints) do
			v:show(player)
		end
	end
	SZEMain.gurneys:updatePlayerGurneys(player)
end

szeOnExitEvent = function(player)
	Karma.ClearContainer(player)
	for k, v in pairs(SZEMain.activeWaypoints) do
		v:hide(player)
	end
	SZEMain.gurneys:clearPlayerGurneys(player)
end

szeOnExitMap = function(player)
	DelayedUIRemover:hideCurrentUI(player)
	Message.FadeSound(player, "Music", 0)
	szeOnExitEvent(player)
end

szeOnEnterVolume = function(player, volume)
	if volume == SZEMain.eventVolume then
		szeOnEnterEvent(player)
	end

	local teleports = SZEMain.teleportsByVolume[volume]
	if teleports then
		for k, v in pairs(teleports) do
			if v.active then
				v:teleport(player)
				break
			end
		end
	end
end

szeOnExitVolume = function(player, volume)
	if volume == SZEMain.eventVolume then
		DelayedUIRemover:add(player)
		Message.ResetMusic(player)
		szeOnExitMap(player)
	end
end

szeOnEntityCreated = function(player)
	local owner = AI.GetOwner(player)
	if SZEMain.entityPetTeams[owner] then
		Team.SetScriptTeam(player, SZEMain.entityPetTeams[owner])
	end
	for k,v in pairs(SZEMain.counters) do
		v:entityCreateIncrement(player)
	end
end

szeOnEntityFreed = function(player)
	SZEMain.entityPetTeams[player] = nil
end

szeOnEntityDefeated = function(player, victor)
	if Entity.IsCritter(player) then
		local objectiveList = SZEMain.karmaObjectives["T"]
		if objectiveList then
			for k, v in pairs(objectiveList) do
				if Entity.IsOnScriptTeam(player, k) then
					for k2, v2 in pairs(v) do
						v2:credit(victor, player)
					end
				end
			end
		end
		objectiveList = SZEMain.karmaObjectives["VD"][Entity.GetVillainDefName(player)]
		if objectiveList then
			for k, v in pairs(objectiveList) do
				v:credit(victor, player)
			end
		end
		objectiveList = SZEMain.karmaObjectives["VG"][Entity.GetVillainGroupID(player)]
		if objectiveList then
			for k, v in pairs(objectiveList) do
				v:credit(victor, player)
			end
		end
		-- counter to count critter onDefeat
		for k, v in pairs(SZEMain.counters) do
			v:critterDefeatIncrement(player, victor)
		end
	elseif Entity.IsPlayer(player) then
		-- counter to count player onDefeat
		for k, v in pairs(SZEMain.counters) do
			v:playerDefeatIncrement(player, victor)
		end
		SZEMain.gurneys:updatePlayerGurneys(player)
	end
end

function SZEMain:addKarmaObjective (category, subcategory, name, karmaObjective)
	if not self.karmaObjectives[category] then
		self.karmaObjectives[category] = {}
	end
	if not self.karmaObjectives[category][subcategory] then
		self.karmaObjectives[category][subcategory] = {}
	end
	
	if name and category and karmaObjective and not self.karmaObjectives[category][subcategory][name] then
		self.karmaObjectives[category][subcategory][name] = karmaObjective
	elseif self.karmaObjectives[category][subcategory][name] then
		print("KarmaObjective " .. name .. " is already defined in this SZE")
	end
end

function SZEMain:addVillainGroupKarmaObjective (group, name, karmaObjective)
	self:addKarmaObjective("VG", Entity.GetVillainGroupIDFromName(group), name, karmaObjective)
end

function SZEMain:addVillainDefKarmaObjective (villain, name, karmaObjective)
	self:addKarmaObjective("VD", villain, name, karmaObjective)
end

function SZEMain:addTeamDefeatKarmaObjective (team, name, karmaObjective)
	self:addKarmaObjective("T", team, name, karmaObjective)
end

function SZEMain:addGlowieKarmaObjective (glowie, name, karmaObjective)
	self:addKarmaObjective("G", glowie, name, karmaObjective)
end

function szeOnScriptMessage(message)
	local _, event, query, subquery, charEvalResult
	_, _, event, query, subquery = string.find(message, "([^%.]*)%.([^%.]*)%.?([^%.]*)")
	
	if not event or not string.lower(event) == string.lower(SZEMain.eventName) then
		return 0
	end
	
	query = string.lower(query)
	
	if query == "door" then
		if subquery and SZEMain.doors[subquery] then
			Script.CharEvalResultSet(1)
		else
			Script.CharEvalResultSet(0)
		end
	elseif query == "eval" then
		if subquery then
			local var = _ENV[subquery]
			if var == nil then
				Script.CharEvalResultSet(0)
			elseif type(var) == "function" then
				Script.CharEvalResultSet(var())
			else
				Script.CharEvalResultSet(var)
			end
		end
	elseif query == "stop" then
		EndScript()
	elseif query == "goto" then
		if subquery and SZEMain.stages[subquery] then
			SZEMain:transition(SZEMain.stages[subquery])
		end
	elseif query == "getid" then
		Script.CharEvalResultSet(Script.GetID() + 1)
	else
		return 0
	end
	
	return 1
end

LoadLuaFile("Lua/Util.lua")
LoadLuaFile("Lua/SZE/SZEStage.lua")
LoadLuaFile("Lua/SZE/SZEUI.lua")
LoadLuaFile("Lua/SZE/SZEKarma.lua")
LoadLuaFile("Lua/SZE/SZEGlowie.lua")
LoadLuaFile("Lua/SZE/SZETimer.lua")
LoadLuaFile("Lua/SZE/SZETeam.lua")
LoadLuaFile("Lua/SZE/SZETeleport.lua")
LoadLuaFile("Lua/SZE/SZEDelay.lua")
LoadLuaFile("Lua/SZE/SZECounter.lua")
LoadLuaFile("Lua/SZE/SZEWaypoint.lua")
LoadLuaFile("Lua/SZE/SZEDoor.lua")
LoadLuaFile("Lua/SZE/SZEGurney.lua")

szeInit = function()
	SZEMain.gurneys = Gurneys:new()
	Callback.OnPlayerEnteringMap("szeOnEnterMap")
	Callback.OnPlayerExitingMap("szeOnExitMap")
	Callback.OnPlayerEntersVolume("szeOnEnterVolume")
	Callback.OnPlayerLeavesVolume("szeOnExitVolume")
	Callback.OnEntityCreated("szeOnEntityCreated")
	Callback.OnEntityFreed("szeOnEntityCreated")
	Callback.OnEntityDefeated("szeOnEntityDefeated")
	Callback.OnScriptMessage("szeOnScriptMessage")
end
szeInit()

szeTick = function()
	SZEMain:update()
end
