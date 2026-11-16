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
		Sound GiantFoot6 100 100 .60
	End
End


Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound HugeGrass3 80 80 .55
	End
End

Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound HugeDirt2 80 80 .55
	End
End




Condition
	TriggerBits	INWATER
	Event
		Type Local
		At origin
		Sound WaterFoot12 80 80 .60
	End
End


Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound GiantFoot2 80 80 .55
	End
End