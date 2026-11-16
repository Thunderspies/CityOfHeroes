#########################################################
##	jack
##
FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	4

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Shotgun_1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr

	End
End

End