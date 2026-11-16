#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Club_1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound axeout 70 70 .5
	End
End

#############################################################

End