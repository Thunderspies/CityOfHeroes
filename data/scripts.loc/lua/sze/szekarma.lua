KarmaReward = {
	pointsNeeded = 0,
	percentileNeeded = 0,
	numStagesRequired = 0,
	rewardTable = ""
}

function KarmaReward:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

KarmaObjective = {
	volume = nil,
	teamKarmaValue = 0,
	teamKarmaLife = 0,
	karmaValue = 0,
	karmaLife = 0
}

function KarmaObjective:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function KarmaObjective:credit (player, target)
	if not self.volume or Location.EntityInArea(target, "trigger:" .. self.volume) then
		Karma.AddObjective(player, self.teamKarmaValue, self.teamKarmaLife, self.karmaValue, self.karmaLife)
	end
end