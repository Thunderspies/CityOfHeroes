#########################################################
##	jack
##
FxInfo

LifeSpan 0

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WePR
		Geom	GEO_WepR_Baton_2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End