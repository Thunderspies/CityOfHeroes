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
		Geom	GEO_WepR_Assault_Rifle_2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound axeout 40 40 .6
	End
End

#############################################################

End