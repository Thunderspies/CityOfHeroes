Timer = {
	duration = 0,
	startTime = 0,
	bonusTime = 0
}

function Timer:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function Timer:start ()
	self.startTime = Script.Now()
end

function Timer:init ()
	self.startTime = Script.Now()
	self.bonusTime = 0
end

function Timer:uiValue ()
	if self.startTime <= 0 then
		return Script.Now()
	else
		return self.duration + self.startTime + self.bonusTime
	end
end

function Timer:remaining ()
	return self.startTime - Script.Now() + self.duration + self.bonusTime
end

function Timer:value ()
	return Script.Now() - self.startTime
end

function Timer:complete ()
	return self:remaining() < 0 and self.startTime > 0
end

function Timer:running ()
	return self.startTime > 0
end

function Timer:addBonus (time)
	self.bonusTime = self.bonusTime + time
end

function Timer:stop ()
	self.startTime = 0
	self.bonusTime = 0
end