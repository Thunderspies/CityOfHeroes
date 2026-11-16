Stage = {
	name = nil,
	UI = nil,
	awardsKarma = false,
	startup = function(self) end,
	shutdown = function(self) end,
	tick = function(self) end,
	nextStage = nil
}

function Stage:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function Stage:update()
	self:tick()
	
	if self.awardsKarma then
		if type(self.awardsKarma) == "function" then
			if self.awardsKarma() then
				Karma.Update(false)
			end
		else
			Karma.Update(false)
		end
	end
	
	if self.UI then
		self.UI:update()
	end
end