#########################################################
##	jack
##
FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepL
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepL_Shield
	End
End

End