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
		Sound Robofoot5 60 60 .5
	End
End


Condition
	TriggerBits	GRASS
	Event
		Type Local
		At origin
		Sound Robograssfoot1 50 50 .36
	End
End

Condition
	TriggerBits	DIRT
	Event
		Type Local
		At origin
		Sound Robodirtfoot1 50 50 .38
	End
End


Condition
	TriggerBits	GRAVEL
	Event
		Type Local
		At origin
		Sound RoboGravelfoot1 58 58 .32
	End
End

Condition
	TriggerBits	METAL
	Event
		Type Local
		At origin
		Sound RoboMetalFoot1 58 58 .33
	End
End




Condition
	TriggerBits	WOOD
	Event
		Type Local
		At origin
		Sound RoboWoodFoot1 58 58 .33
	End
End


Condition
	TriggerBits	INWATER
	Event
		Type Local
		At origin
		Sound robowaterfoot1 50 50 .60
	End
End


Condition
	On	DEFAULTSURFACE
	Event
		Type Local
		At origin
		Sound RoboFoot5 60 60 .55
	End
End