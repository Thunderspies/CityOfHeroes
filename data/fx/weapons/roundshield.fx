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
		Geom	GEO_WepL_Shield_round
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End