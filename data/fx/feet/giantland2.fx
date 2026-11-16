#########################################################
##	MH
## 	Male Lands

FxInfo

Input
	InpName Origin
End

LifeSpan 5
Condition
	TriggerBits	CONCRETE
	Event
		Type Local
		At origin
		Sound Land3 50 50 .40
	End
End

Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound GrassLand1 30 30 .25
	End
End


Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound HugeDirtLand 30 30 .22
	End
End

Condition
	TriggerBits	GRAVEL
	Event
		Type Local
		At origin
		Sound GravelLand1 30 30 .39
	End
End

Condition
	TriggerBits	WOOD
	Event
		Type Local
		At origin
		Sound WoodHugeLand 30 30 .43
	End
End


Condition
	TriggerBits	METAL
	Event
		Type Local
		At origin
		Sound MetalHugeLand2 30 30 .6
	End
End


Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound Land4 50 50 .7
	End
End