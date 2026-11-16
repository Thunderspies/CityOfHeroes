GlowieSet = {
	glowieDef = nil,
	location = nil,
	reuseLocations = true,
	vanishOnComplete = false,
	numLoc = nil,
	quantity = nil,
	displayName = nil,
	model = nil,
	reward = nil,
	charRequires = nil,
	charRequiresFailedText = nil,
	interactText = "",
	interruptText = "",
	completeText = "",
	barText = "",
	interactTime = 0,
	effects = {},
	glowieStates = {},
	glowieEntites = {}, -- links to same GlowieStates as in glowieStates but indexed by entity,
	placed = 0
}

GlowieState = {
	entity = nil,
	location = nil,
	available = true,
	active = false,
	timer = 0
}

function GlowieState:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function GlowieSet:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function GlowieSet:init ()
	self.glowieDef = Glowie.CreateDef(self.displayName, self.model, self.interactText, self.interruptText, self.completeText, self.barText, self.interactTime)
	if self.charRequires then
		Glowie.SetCharRequires(self.glowieDef, self.charRequires, self.charRequiresFailedText)
	end
	self.glowieEntities = {}
	self.numLoc = Location.CountMarkers(self.location)
	if self.numLoc <= 0 and Location.Exists("marker:" .. self.location) then
		self.numLoc = 1
		self.glowieStates = { GlowieState:new{location = "marker:" .. self.location} }
	elseif self.numLoc > 0 then
		self.glowieStates = {}
		for i=1,self.numLoc do
			local markerName = string.format("marker:%s_%02d", self.location, i)
			table.insert(self.glowieStates, GlowieState:new{location = markerName})
		end
	end
	self.quantity = math.max(1, self.quantity or 0)
	self.quantity = math.min(self.quantity, self.numLoc)
end

function GlowieSet:place ()
	if not self.glowieDef then -- our definition has become deallocated by COH's code, we need to recreate it in COH's code
		self:init()
	end

	local i
	for i = #self.glowieStates, 2, -1 do -- randomize the glowie array
		local r = math.random(i)
		self.glowieStates[i], self.glowieStates[r] = self.glowieStates[r], self.glowieStates[i]
	end
	
	local cur = 1
	for i = 1, self.quantity do
		while cur <= #self.glowieStates and not self.glowieStates[cur].available do
			cur = cur + 1
		end
		if cur <= #self.glowieStates then
			local selectedGlowie = self.glowieStates[cur]
			if not selectedGlowie.entity then
				selectedGlowie.entity = Glowie.Place(self.glowieDef, selectedGlowie.location, 0, "szeOnGlowieClick", "szeOnGlowieComplete")
				self.glowieEntities[selectedGlowie.entity] = selectedGlowie
				self.placed = self.placed + 1
			end
			selectedGlowie.available = false
			cur = cur + 1
			Glowie.ClearState(selectedGlowie.entity)
		else
			break
		end
	end
end

function GlowieSet:activate()
	for k, v in pairs(self.glowieEntities) do
		Glowie.SetState(k)
		v.active = true
		v.timer = 0
	end
end

function GlowieSet:deactivate()
	for k, v in pairs(self.glowieEntities) do
		if v.active then
			Glowie.ClearState(k)
			v.active = false
			v.timer = 0
		end
	end
end

function GlowieSet:remove()
	local i
	for i = 1, #self.glowieStates do
		if self.glowieStates[i].entity then
			Glowie.ClearState(self.glowieStates[i].entity)
			Glowie.Remove(self.glowieStates[i].entity)
			self.glowieStates[i].entity = nil
			self.placed = self.placed - 1
			if self.placed == 0 then		-- COH has deallocated this definition, we will need to reallocate it if we want to place this again (calling Glowie.CreateDef)
				self.glowieDef = nil
			end
		end
		self.glowieStates[i].active = false
		self.glowieStates[i].timer = 0
		if self.reuseLocations then
			self.glowieStates[i].available = true
		end
	end
end

function GlowieSet:clearUsedLocations()
	local i
	for i = 1, #self.glowieStates do
		if not self.glowieStates[i].active and not self.glowieStates[i].available then
			self.glowieStates[i].available = true
		end
	end
end

function GlowieSet:complete(name, player, target)
	local selectedGlowie = self.glowieEntities[target]
	if selectedGlowie then
		if SZEMain.karmaObjectives["G"][name] then
			for k, v in pairs(SZEMain.karmaObjectives["G"][name]) do
				SZEMain:foreachPlayerInEvent( function(member) Karma.GlowieBubbleActivate(player, member) end )
				v:credit(player, target)
			end
		end
		-- find/increment glowie counter
		for k, v in pairs(SZEMain.counters) do
			v:glowieCompleteIncrement(player, self.displayName)
		end
		if self.reward then
			Reward.GrantTable(player, self.reward)
		end
		if self.resetDelay and self.resetDelay > 0 then
			selectedGlowie.timer = Script.Now() + self.resetDelay
			Glowie.ClearState(target)
			if self.vanishOnComplete then 
				Glowie.Remove(target)
				selectedGlowie.entity = nil
				self.glowieEntities[target] = nil
				self.placed = self.placed - 1
				if self.placed == 0 then		-- COH has deallocated this definition, we will need to reallocate it if we want to place this again (calling Glowie.CreateDef)
					self.glowieDef = nil
				end
			end
		else
			Glowie.SetState(target)
		end
	end
end

function GlowieSet:update()
	local i
	for i = 1, #self.glowieStates do
		local selectedGlowie = self.glowieStates[i]
		if selectedGlowie.timer > 0 and Script.Now() > selectedGlowie.timer then
			if not selectedGlowie.entity then
				if not self.glowieDef then -- our definition has become deallocated by COH's code, we need to recreate it in COH's code
					self:init()
				end
			
				selectedGlowie.entity = Glowie.Place(self.glowieDef, selectedGlowie.location, 0, "szeOnGlowieClick", "szeOnGlowieComplete")
				self.glowieEntities[selectedGlowie.entity] = selectedGlowie
				self.placed = self.placed + 1
			end
			Glowie.SetState(selectedGlowie.entity)
			selectedGlowie.timer = 0
		end
	end
end

function szeOnGlowieClick(player, target)
end

function szeOnGlowieComplete(player, target)
	for k, v in pairs(SZEMain.glowies) do
		v:complete(k, player, target)
	end
end