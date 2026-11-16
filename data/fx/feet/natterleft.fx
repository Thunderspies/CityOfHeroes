#########################################################
##	MH
## 	Male Left Foot

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
		Sound Boot5 65 65 .24
	End
End




Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound Grass6 55 55 .10
	End
End



Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound Dirt3 55 55 .10
	End
End



Condition
	TriggerBits	GRAVEL
	Event
		Type Local
		At origin
		Sound Gravelfoot3 55 55 .10
	End
End





Condition
	TriggerBits	INWATER
	Event
		Type Local
		At origin
		Sound WaterFoot11 55 55 .45
	End
End




Condition
	TriggerBits	METAL
	Event
		Type Local
		At origin
		Sound Metal1 55 55 .33
	End
End



Condition
	TriggerBits	WOOD
	Event
		Type Local
		At origin
		Sound WoodFoot2 55 55 .33
	End
End



Condition
	TriggerBits	CARPET
	Event
		Type Local
		At origin
		Sound Carpet4 45 45 .54
	End
End



Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound Boot5 65 65 .24
	End
End



