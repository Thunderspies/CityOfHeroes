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
		Geom	GEO_WepR_Pistol_2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#############################################################

End