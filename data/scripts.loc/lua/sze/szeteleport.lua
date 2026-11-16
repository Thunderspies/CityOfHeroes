Teleport = {
	active = false,
	volume = nil,
	destination = nil,
}

function Teleport:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function Teleport:activate ()
	if self.volume and self.destination then
		self.active = true
		SZEMain:foreachPlayerInVolume( function(player) self:teleport(player) end, self.volume)
	end
end

function Teleport:deactivate ()
	self.active = false
end

function Teleport:teleport (player)
	if self.destination then
		Entity.PlayFX(player, "POWERS/Teleportation/TeleportRecall.fx")
		Delay.delay(1, function() Team.TeleportToLocation(player, self.destination) end, false)
	end
end