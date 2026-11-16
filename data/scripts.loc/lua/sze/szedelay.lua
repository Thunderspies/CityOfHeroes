Delay = {
	time = nil,
	action = nil,
	clearOnTransition = nil
}

function Delay:new(o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function Delay.delay(amount, action, clearOnTransition, delayedActionList)
	if not action or not type(action) == "function" then
		return
	end
	delayedActionList = delayedActionList or SZEMain.delayedActions
	
	local delayedAction = Delay:new{ time = Script.Now() + amount, action = action, clearOnTransition = clearOnTransition }
	table.insert(delayedActionList, delayedAction)
end

function Delay.processDelayedActions(actions)
	local currentTime = Script.Now()
	local i = 1
	while actions[i] do
		local action = actions[i]
		if currentTime >= action.time then
			action.action()
			table.remove(actions, i)
		else
			i = i + 1
		end
	end
end


function Delay.clearDelayedActionsOnTransition(actions)
	local i = 1
	while actions[i] do
		local action = actions[i]
		if action.clearOnTransition then
			table.remove(actions, i)
		else
			i = i + 1
		end
	end
end