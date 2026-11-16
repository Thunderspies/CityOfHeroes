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
		Geom	GEO_WepR_Submachine_2
		Sound axeout 40 40 .6
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#############################################################

End