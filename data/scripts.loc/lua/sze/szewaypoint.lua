SZEWaypoint = {
	location = nil,
	text = nil,
	icon = nil,
	iconB = nil,
	compass = false,
	pulse = false,
	rotate = -1,
	dynamic = false,
	active = false,
	id = nil
}

function SZEWaypoint:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function SZEWaypoint:init ()
	if self.id then
		return
	end
	
	self.id = Waypoint.Create(self.location, self.text, self.icon, self.iconB, self.compass, self.pulse, self.rotate)
end

function SZEWaypoint:show (team)
	if not self.id then
		self:init()
	end
	
	if self.id then
		Waypoint.Set(team, self.id)
	end
end

function SZEWaypoint:hide (team)
	if self.id then
		Waypoint.Clear(team, self.id)
	end
end

function SZEWaypoint:update ()
	if self.active and self.dynamic then
		Waypoint.Update(self.id, self.location, self.text, self.icon, self.rotate)
	end
end

function SZEWaypoint:activate ()
	if not self.active then
		if not self.id then
			self:init()
		end
		
		SZEMain:foreachPlayerInEvent ( function(player) self:show(player) end )
		self.active = true
		SZEMain.activeWaypoints[self.id] = self
	end
end

function SZEWaypoint:deactivate ()
	if self.id then
		Waypoint.Destroy(self.id)
		self.active = false
		SZEMain.activeWaypoints[self.id] = nil
		self.id = nil
	end
end
	