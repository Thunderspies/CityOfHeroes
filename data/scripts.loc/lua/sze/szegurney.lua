Gurney = {
	active = false,
	volume = nil,
	target = nil
}

function Gurney:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function Gurney:update (player)
	if self.active and self.target then
		if not self.volume or Location.EntityInArea(player, "trigger:" .. self.volume) then
			Entity.OverrideGurney(player, false, self.target)
			return true
		end
	end
	return false
end

Gurneys = {
	gurneys = {},
	eventGurneys = {},
	volumeGurneys = {}
}

function Gurneys:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function Gurneys:add (name, gurney)
	if name and gurney and not self.gurneys[name] then
		self.gurneys[name] = gurney
		if gurney.volume then
			self.volumeGurneys[name] = gurney
		else
			self.eventGurneys[name] = gurney
		end
	elseif name and gurney then
		print("Gurney " .. name .. " is already defined in this SZE")
	end
end

function Gurneys:activate(name)
	if name and self.gurneys[name] and not self.gurneys[name].active then
		self.gurneys[name].active = true
		self:updateAllGurneys()
	end
end

function Gurneys:deactivate(name)
	if name and self.gurneys[name] and self.gurneys[name].active then
		self.gurneys[name].active = false
		self:updateAllGurneys()
	end
end

function Gurneys:updatePlayerGurneys(player)
	for k, v in pairs(self.volumeGurneys) do
		if v:update(player) then
			return
		end
	end
	
	for k, v in pairs(self.eventGurneys) do
		if v:update(player) then
			return
		end
	end
	
	-- no active gurney overrides, so clear any previously set
	self:clearPlayerGurneys(player)
end

function Gurneys:updateAllGurneys()
	SZEMain:foreachPlayerInEvent( function(player) self:updatePlayerGurneys(player) end, true )
end

function Gurneys:clearPlayerGurneys(player)
	Entity.OverrideGurney(player, false, nil)
end