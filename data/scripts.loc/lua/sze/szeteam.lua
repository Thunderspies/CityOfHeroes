Fountain = {
	teamSizeCount = { {min = 0, full = 0} },
	minRespawnTime = nil,
	maxRespawnTime = nil,
	lastSpawnTime = nil,
	nextSpawnTime = nil
}

function Fountain:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	o.teamSizeCount = { {min = 0, full = 0} }
	self.__index = self
	return o
end

function Fountain:addTeamSizeCount(players, min, full)
	self.teamSizeCount[players] = {min = min, full = full}
end

function Fountain:getTeamSizeCount(players)
	local min, full = 0, 0
	for i, v in pairsByKeys(self.teamSizeCount) do
		if players >= i then
			min = v.min
			full = v.full
		end
	end
	return min, full
end

function Fountain:updateLastSpawnTime()
	self.nextSpawnTime = nil
	self.lastSpawnTime = Script.Now()
end

function Fountain:checkRespawnPop(currentPop, players)
	if not self.maxRespawnTime and not self.minRespawnTime then
		return
	end
	
	min, full = self:getTeamSizeCount(players)
	if currentPop < full and self.maxRespawnTime then
		-- prepare for a spawn based upon the max respawn timer
		respawnTime = Script.Now() + self.maxRespawnTime
		if self.nextSpawnTime == nil or respawnTime < self.nextSpawnTime then
			self.nextSpawnTime = respawnTime
		end
	end
	
	if min > currentPop and self.minRespawnTime then
		-- prepare for a spawn based upon the min respawn timer
		respawnTime = Script.Now() + self.minRespawnTime
		if self.nextSpawnTime == nil or respawnTime < self.nextSpawnTime then
			self.nextSpawnTime = respawnTime
		end
	end
end

function Fountain:checkRespawnTimer()
	return self.nextSpawnTime and self.nextSpawnTime <= Script.Now()
end

function Fountain:printFountain()
	for k, v in pairs(self.teamSizeCount) do
		print("Players:", k, "Min:", v.min, "Full:", v.full)
	end
end

SZETeam = {
	name = nil,
	layout = nil,
	spawnBehavior = nil,
	killBehavior = nil,
	petTeam = nil,
	active = false,
	fountain = nil,
	useActivePlayers = true,
	onCreate = nil
}

function SZETeam.attachToTeam (creatorTeam)
	return function(self, newTeam)
		local n = Team.NumEntitiesInTeam(creatorTeam)
		if n > 0 then
			self.creatorEnt = Team.GetEntityFromTeam(creatorTeam, math.random(n))
			AI.SetFollowers(self.creatorEnt, newTeam)
		end
	end
end

function SZETeam.attachToVillainDef (villainDef)
	return function(self, newTeam)
		self.creatorEnt = Entity.GetVillainByDefName(villainDef, self.creatorEnt)
		if self.creatorEnt then
			AI.SetFollowers(self.creatorEnt, newTeam)
		end
	end
end
	
function SZETeam:fireAllSpawns(players)
	for i = 1, #(self.encountergroups) do
		Encounter.Spawn(1, tempCreationTeam, nil, self.encountergroups[i], nil, nil, players)
	end
	
	self:setupNewTeam(tempCreationTeam)
end

function SZETeam:fireSingleSpawn(index, players)
	Encounter.Spawn(1, tempCreationTeam, nil, self.encountergroups[index], nil, nil, players)
	
	self:setupNewTeam(tempCreationTeam)
end

function SZETeam:setupNewTeam(newTeam)
	if self.onCreate and type(self.onCreate) == "function" then
		self:onCreate(newTeam)
	end
	
	if self.spawnBehavior then
		AI.AddBehavior(newTeam, self.spawnBehavior)
	end
	
	if self.fountain then
		Encounter.DetachSpawn(newTeam)
	end
	
	if self.petTeam then
		local n = Team.NumEntitiesInTeam(newTeam)
		for i = 1, n do
			local newEntity = Team.GetEntityFromTeam(newTeam, i)
			SZEMain.entityPetTeams[newEntity] = self.petTeam
		end
	end
	
	Team.SwitchScriptTeam(newTeam, newTeam, self.name)
end

function SZETeam:fountainPopulate(players)
	if not self.fountain then
		return
	end
	
	self.fountain:updateLastSpawnTime()
	
	min, full = self.fountain:getTeamSizeCount(players)
	
	randomGroup = math.random(#self.encountergroups)
	maxLoops = 100
	i = 0
	
	while Team.NumEntitiesInTeam(self.name) < full and i < maxLoops do
		self:fireSingleSpawn(randomGroup, players)
		randomGroup = randomGroup + 1
		if randomGroup > #self.encountergroups then randomGroup = 1 end
		i = i + 1
	end
end

function SZETeam:activate()
	if not self.active then
		if not self.encountergroups or not (#self.encountergroups > 0) then
			print("Cannot locate encounter groups for SZETeam " .. self.name)
			return
		end
		if not self.fountain then
			self:fireAllSpawns(SZEMain:playerCount(self.useActivePlayers))
		else
			self:fountainPopulate(SZEMain:playerCount(self.useActivePlayers))
			SZEMain.activeFountainTeams[self.name] = self
		end
		self.active = true
	end
end

function SZETeam:deactivate()
	if self.killBehavior then
		AI.AddBehavior(self.name, self.killBehavior)
	end
	SZEMain.activeFountainTeams[self.name] = nil
	self.active = false
end

function SZETeam:forceSpawn()
	if not self.fountain then
		Encounter.DetachSpawn(self.name)
		self:fireAllSpawns(SZEMain:playerCount(self.useActivePlayers))
	else
		Team.SwitchScriptTeam(self.name, self.name, forceCreationTeam)
		self:fountainPopulate(SZEMain:playerCount(self.useActivePlayers))
		Team.SwitchScriptTeam(forceCreationTeam, forceCreationTeam, self.name)
	end
end

function SZETeam:doBehavior(behavior)
	AI.AddBehavior(self.name, behavior)
end

function SZETeam:detach()
	Encounter.DetachSpawn(self.name)
end

function SZETeam:setHealth(health)
	if self.active then
		local i, n = Team.NumEntitiesInTeam(self.name)
		for i = 1, n do
			Entity.SetHealth(Team.GetEntityFromTeam(teamName, i), health, 0)
		end
	end
end

function SZETeam:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	if o.layout then
		o.encountergroups = Encounter.FindAllMatchingEncounterGroups(nil, nil, nil, nil, o.layout, nil)
	end
	return o
end