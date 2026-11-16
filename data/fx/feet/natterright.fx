#########################################################
##	MH
## 	Male Right Foot

FxInfo

Input
	InpName Origin
End

LifeSpan 1




Condition
	TriggerBits	CONCRETE
	Event
		Type Local
		At origin
		Sound boot6 65 65 .25
	End
End


Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound Grass7 55 55 .10
	End
End

Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound Dirt4 55 55 .10
	End
End



Condition
	TriggerBits	GRAVEL
	Event
		Type Local
		At origin
		Sound Gravelfoot2 55 55 .10
	End
End

Condition
	TriggerBits	INWATER
	Event
		Type Local
		At origin
		Sound WaterFoot12 55 55 .43
	End
End


Condition
	TriggerBits	METAL
	Event
		Type Local
		At origin
		Sound Metal2 55 55 .33
	End
End

Condition
	TriggerBits	WOOD
	Event
		Type Local
		At origin
		Sound WoodFoot1 55 55 .33
	End
End

Condition
	TriggerBits	CARPET
	Event
		Type Local
		At origin
		Sound Carpet3 45 45 .55
	End
End


Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound boot6 65 65 .25
	End
End
