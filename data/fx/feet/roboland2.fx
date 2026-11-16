#########################################################
##	MH
## 	Huge Lands

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
		Sound Roboland2 70 70 .6
	End
End



Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound RoboGrassLand1 50 50 .46
	End
End



Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound RoboDirtLand 50 50 .56
	End
End



Condition
	TriggerBits	WOOD
	Event
		Type Local
		At origin
		Sound RoboWoodLand 50 50 .54
	End
End




Condition
	TriggerBits	GRAVEL
	Event
		Type Local
		At origin
		Sound RoboGravelLand 50 50 .44
	End
End



Condition
	TriggerBits	METAL
	Event
		Type Local
		At origin
		Sound RoboMetalLand 50 50 .5
	End
End



Condition
	TriggerBits	CARPET
	Event
		Type Local
		At origin
		Sound HugeCarpetland 50 50 .99
	End
End



Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound Roboland2 70 70 .6
	End
End