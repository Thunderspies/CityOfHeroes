BOSS_MAX_HEALTH = 100

Counter = 
{
	ctype = nil,
	direction = nil,
	villainGroup = nil,
	team = nil,
	name = nil,
	volumeName = nil,
	glowie = nil,
	-- displayProgress = 0,
	target = 0,
	progress = 0,
}

function Counter:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function Counter:init()
	if self.direction == "Down" and self.team then
		self.progress = Team.NumEntitiesInTeam(self.team)
	else 
		self.progress = 0
	end
end

function Counter:complete()
	return self.progress == self.target
end

function Counter:critterDefeatIncrement(player, victor)
	-- for villain group kill counters with a meaningful villain group
	if self.ctype == "VillainGroup" and self.villainGroup then
		desiredID = Entity.GetVillainGroupIDFromName(self.villainGroup)
		victimID = Entity.GetVillainGroupID(player)
		-- If the victim's group matches
		if desiredID and desiredID == victimID then
			-- If a volume was given, make sure he's in it.
			if not self.volumeName or victor == "TEAM_NONE" or Location.EntityInArea(victor, "trigger:" .. self.volumeName) then
				-- // At last.  Something to count.  But only if we didn't hit the target
				if self.progress < self.target then
					self.progress = self.progress + 1
				end
			end
		end
	-- for team kill counters with a meaningful team
	elseif self.ctype == "Team" and self.team then
		if Entity.IsOnScriptTeam(player, self.team) then
			if self.direction == "Down" then
				self.progress = Team.NumEntitiesInTeam(self.team)
			else
				-- At last.  Something to count.  But only if we didn't hit the target
				if self.progress < self.target then
					self.progress = self.progress + 1
				end
			end
		end
	-- for named kill counters with a meaningful name
	elseif self.ctype == "Name" and self.name then
		if Entity.GetVillainDefName(player) == self.name then
			if self.direction == "Down" then
				self.progress = self.progress - 1
				if self.progress < 0 then
					-- Named counter progress was negative
					self.progress = 0
				end
			else
				if self.progress < self.target then
					self.progress = self.progress + 1
				end
			end	
		end
	end
end

function Counter:playerDefeatIncrement(player, victor)
	if self.ctype == "PlayerDeaths" then
		if counter.direction == "Down" then
			self.progress = self.progress - 1
			if self.progress < 0 then
				--	the number of deaths may actually go below zero if multiple people die in the same tick before
				--	any designer intended stage change. just floor it to zero instead of letting it go negative
				self.progress = 0
			end
		else
			if self.progress < self.target then
				self.progress = self.progress + 1
			end
		end
	end
end

function Counter:entityCreateIncrement(player)
	if self.ctype == "Name" and self.name then
		if Entity.GetVillainDefName(player) == self.name then
			if self.direction == "Down" then
				self.progress = self.progress + 1
			end
		end
	end
end

function Counter:glowieCompleteIncrement(player, glowieName)
	if self.ctype == "Click" and self.glowie then
		if self.glowie == glowieName then
			if self.progress < self.target then
				self.progress = self.progress + 1
			end
		end
	end
end

-- function Counter:ZEPlayerUpdateIncrement()
	-- local activeTeam
	-- if self.volumeName then
		-- activeTeam = GetScriptVolumeTeam(self.volumeName)
	-- else
		-- activeTeam = GetScriptTeam()
	-- end
	-- if self.ctype == "ZEPlayers" then
		-- self.progress = Team.NumEntitiesInTeamEvenDead(activeTeam)
	-- elseif self.ctype == "ZEPlayersAlive" then
		-- self.progress = 0
		-- for i = 0, Team.NumEntitiesInTeam(activeTeam) do
			-- if Entity.GetHealth(Team.GetEntityFromTeam(activeTeam, i - 1)) > 0 then
				-- self.progress = self.progress + 1
			-- end
		-- end
	-- elseif self.ctype == "ZEPlayersPhased" then
		-- self.progress = 0
		-- for i = 0, Team.NumEntitiesInTeam(activeTeam) do
			-- if Entity.IsPhased(Team.GetEntityFromTeam(activeTeam, i - 1)) then
				-- self.progress = self.progress + 1
			-- end
		-- end
	-- elseif self.ctype == "ZEPlayersActive" then
		-- self.progress = 0
		-- for i = 0, Team.NumEntitiesInTeam(activeTeam) do
			-- if Entity.IsActive(Team.GetEntityFromTeam(activeTeam, i - 1)) then
				-- self.progress = self.progress + 1
			-- end
		-- end
	-- end
-- end

-- function Counter:stageIncrement()
	-- if self.ctype == "BossHealth" then
		-- if self.team then
			-- local bossHp = Entity.GetHealth(self.team)
			-- self.progress = math.ceil(bossHp * BOSS_MAX_HEALTH)
		-- end
	-- elseif self.ctype == "Team" then
		-- if self.direction == "Down" and self.team then
			-- self.progress = Team.NumEntitiesInTeam(self.team)
		-- end
	-- end
-- end