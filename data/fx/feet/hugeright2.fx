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
		Sound Giantfoot6 100 100 .33
	End
End


Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound Grass5 88 88 .19
	End
End

Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound HugeDirt2 88 88 .13
	End
End



Condition
	TriggerBits	GRAVEL
	Event
		Type Local
		At origin
		Sound Gravelfoot2 88 88 .13
	End
End

Condition
	TriggerBits	INWATER
	Event
		Type Local
		At origin
		Sound WaterFoot12 70 70 .43
	End
End


Condition
	TriggerBits	METAL
	Event
		Type Local
		At origin
		Sound Metal6 25 25 .33
	End
End

Condition
	TriggerBits	WOOD
	Event
		Type Local
		At origin
		Sound WoodHuge3 78 78 .33
	End
End


Condition
	TriggerBits	CARPET
	Event
		Type Local
		At origin
		Sound Carpet4 50 50 .54
	End
End

Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound giantfoot6 100 100 .39
	End
End
