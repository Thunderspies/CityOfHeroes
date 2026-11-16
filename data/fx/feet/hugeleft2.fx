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
		Sound giantfoot5 100 100 .33
	End
End




Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound Grass7 88 88 .17
	End
End




Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound HugeDirt1 88 88 .16
	End
End




Condition
	TriggerBits	GRAVEL
	Event
		Type Local
		At origin
		Sound Gravelfoot1 88 88 .12
	End
End




Condition
	TriggerBits	INWATER
	Event
		Type Local
		At origin
		Sound WaterFoot11 70 70 .45
	End
End




Condition
	TriggerBits	METAL
	Event
		Type Local
		At origin
		Sound Metal5 88 88 .33
	End
End




Condition
	TriggerBits	WOOD
	Event
		Type Local
		At origin
		Sound WoodHuge2 88 88 .33
	End
End




Condition
	TriggerBits	CARPET
	Event
		Type Local
		At origin
		Sound Carpet3 50 50 .54
	End
End



Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound giantfoot5 100 100 .29
	End
End