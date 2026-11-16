#########################################################
##	MH
## 	Male Left Foot

FxInfo

Input
	InpName Origin
End

LifeSpan 12



Condition
	TriggerBits	CONCRETE
	Event
		Type Local
		At origin
		Sound Giantfoot1 160 160 .55
	End
End


Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound HugeGrass1 130 130 .55
	End
End

Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound HugeDirt1 130 130 .55
	End
End




Condition
	TriggerBits	INWATER
	Event
		Type Local
		At origin
		Sound WaterFoot11 120 120 .60
	End
End


Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound GiantFoot1 130 130 .55
	End
End