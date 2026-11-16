#########################################################
##	MH
## 	Male Left Foot

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
		Sound Robofoot6 60 60 .5
	End
End


Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound Robograssfoot2 50 50 .35
	End
End

Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound RoboDirtfoot2 50 50 .35
	End
End




Condition
	TriggerBits	INWATER
	Event
		Type Local
		At origin
		Sound Robowaterfoot2 50 50 .60
	End
End

Condition
	TriggerBits	GRAVEL
	Event
		Type Local
		At origin
		Sound RoboGravelfoot2 58 58 .32
	End
End

Condition
	TriggerBits	METAL
	Event
		Type Local
		At origin
		Sound RoboMetalFoot2 58 58 .33
	End
End




Condition
	TriggerBits	WOOD
	Event
		Type Local
		At origin
		Sound RoboWoodFoot2 58 58 .33
	End
End


Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound RoboFoot6 60 60 .5
	End
End