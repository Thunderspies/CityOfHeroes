#########################################################
##	jack
##
FxInfo

Flags InheritAlpha DontSuppress

LifeSpan	30

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Knife_2
	End
End

End