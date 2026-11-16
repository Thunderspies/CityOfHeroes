#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5


	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Axe_1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound axeout 60 60 .6
	End
End

End