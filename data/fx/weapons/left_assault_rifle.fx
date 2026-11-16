#########################################################
##	jack
##
FxInfo

LifeSpan 0

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	4

	Event
		EName 	Prime
		Type	Local
		At	WepL
		Geom	GEO_WepR_Machine_Gun_1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End



