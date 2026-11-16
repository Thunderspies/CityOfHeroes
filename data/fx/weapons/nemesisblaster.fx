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
		At	WepR
		Geom	GEO_WepR_Nemesis_02
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 80 80 .6

	End
End

End