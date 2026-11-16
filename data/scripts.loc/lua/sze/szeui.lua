DelayedUIRemover = {
	players = {}
}

function DelayedUIRemover:update()
	curTime = Script.Now()
	for k, v in pairs(self.players) do
		if v >= curTime then
			self:hideCurrentUI(player)
		end
	end
end

function DelayedUIRemover:hideCurrentUI(player)
	UICollection.hideCurrentUI(player)
	self.players[player] = nil
end

function DelayedUIRemover:add(player)
	self.players[player] = Script.Now() + 5
end

function DelayedUIRemover:remove(player)
	self.players[player] = nil
end

UICollections = {
	collections = {},
	currentCollection = nil,
	volume = nil
}

function UICollections:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function UICollections:init()
	if not self.currentCollection then
		for i, v in ipairs(self.collections) do
			v:init()
		end
		for i, v in ipairs(self.collections) do
			if v:display() then
				self.currentCollection = i
				v:show(self.volume)
				break
			end
		end
	else
		self.collections[self.currentCollection]:show(self.volume)
	end
end

function UICollections:update()
	if self.currentCollection then
		self.collections[self.currentCollection]:update()
	end
end

function UICollections:change()
	for i, v in ipairs(self.collections) do
		if v:display() and i ~= self.currentCollection then
			self.currentCollection = i
			v:show(self.volume)
			break
		end
	end
end

function UICollections:showPlayer(player)
	if not self.currentCollection then
		self:init()
	else
		self.collections[self.currentCollection]:showPlayer(player, self.volume)
	end
end

UICollection = {
	name = "",
	scriptDisplayName = "",
	stageDisplayName = "",
	stageTooltip = "",
	display = function() return true end,
	items = {},
	widgets = {},
	updateTick = 0
}

function UICollection:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

function UICollection:init()
	if not self.name then
		return
	elseif not UI.CollectionExists("Collection:" .. self.name) then
		self.widgets = {}
	
		uiScriptTitle = self.name .. "ScriptTitle"
		Var.SetHidden(uiScriptTitle, self.scriptDisplayName)
		uiScriptWidget = "Script" .. self.name
		UI.Title(uiScriptWidget, uiScriptTitle, "")
		table.insert(self.widgets, uiScriptWidget)
		
		uiStageTitle = self.name .. "StageTitle"
		Var.SetHidden(uiStageTitle, self.stageDisplayName)
		uiStageWidget = "Stage" .. self.name
		UI.List(uiStageWidget, self.stageTooltip, uiStageTitle, "ffffffff", "", "ffffffff")
		table.insert(self.widgets, uiStageWidget)
		
		for i, v in ipairs(self.items) do
			if not v.widgetName then
				v.widgetName = self.name .. "UIWidget" .. i
			end
			if not v.currentVar then
				v.currentVar = v.widgetName .. "CurVar"
			end
			if not v.targetVar then
				v.targetVar = v.widgetName .. "TgtVar"
			end
			if not v.findVar then
				v.findVar = v.widgetName .. "FindVar"
			end
			
			if v.uiType == "BarBlue" then
				UI.MeterEx(v.widgetName, v.text, v.shortText, v.currentVar, v.targetVar, "Blue", v.tooltip, v.findVar)
			elseif v.uiType == "BarGold" then
				UI.MeterEx(v.widgetName, v.text, v.shortText, v.currentVar, v.targetVar, "Gold", v.tooltip, v.findVar)
			elseif v.uiType == "BarRed" then
				UI.MeterEx(v.widgetName, v.text, v.shortText, v.currentVar, v.targetVar, "Red", v.tooltip, v.findVar)
			elseif v.uiType == "Text" then
				UI.Text(v.widgetName, v.text, "", v.tooltip)
			elseif v.uiType == "CenterText" then
				UI.Text(v.widgetName, v.text, "center", v.tooltip)
			elseif v.uiType == "Timer" then
				UI.Timer(v.widgetName, v.currentVar, v.text, v.tooltip)
			end
			
			v.visibleLastTick = true
				
			table.insert(self.widgets, v.widgetName)
		end
		
		UI.CreateCollectionEx(self.name, #self.widgets, self.widgets)
		self:update(true)
	end
end

function UICollection:update(force)
	if self.updateTick >= 1 or force then
		for i, v in ipairs(self.items) do
			if v.display() then
				if not v.visibleLastTick then
					UI.WidgetHidden(v.widgetName, 0)
					v.visibleLastTick = true
				end
				
				Var.SetNumberHidden(v.currentVar, v.currentVal())
				Var.SetNumberHidden(v.targetVar, v.targetVal())
				if v.find then
					UI.SetUpEntityTracking(v.findVar, v.find)
				end
			elseif v.visibleLastTick then
				UI.WidgetHidden(v.widgetName, 1)
				v.visibleLastTick = false
			end
		end
		self.updateTick = 0
	else
		self.updateTick = self.updateTick + 1
	end
end

function UICollection:show(volume)
	n = Team.NumEntitiesInTeam(ALL_PLAYERS_READYORNOT)
	for i=1,n do
		player = Team.GetEntityFromTeam(ALL_PLAYERS_READYORNOT, i)
		self:showPlayer(player, volume)
	end
end

function UICollection:showPlayer(player, volume)
	playerCollection = Var.Get(Var.EntVar(player, "CurrentUI"))
	if playerCollection and not (playerCollection == self.name) then
		UI.Hide("Collection:" .. playerCollection, player)
		Var.SetHidden(Var.EntVar(player, "CurrentUI"), nil)
	end
	if not volume or Location.EntityInArea(player, "trigger:" .. volume) then
		UI.Show("Collection:" .. self.name, player)
		UI.SendDetachCommand(player, 1);
		Var.SetHidden(Var.EntVar(player, "CurrentUI"), self.name)
	end
end

function UICollection.hideCurrentUI(player)
	playerCollection = Var.Get(Var.EntVar(player, "CurrentUI"))
	if playerCollection then
		UI.Hide("Collection:" .. playerCollection, player)
		Var.SetHidden(Var.EntVar(player, "CurrentUI"), nil)
	end
end

UIItem = {
	name = nil,
	uiType = nil,
	text = nil,
	shortText = nil,
	tooltip = "",
	currentVal = function() return 0 end,
	currentVar = nil,
	targetVal = function() return 0 end,
	targetVar = nil,
	display = function() return true end,
	find = nil,
	findVar = nil,
	widgetName = nil,
	visibleLastTick = false
}

function UIItem:new (o)
	o = o or {}   -- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	
	if not o.shortText then
		o.shortText = o.text
	end
	
	return o
end