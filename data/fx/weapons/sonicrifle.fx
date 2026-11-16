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
		At	WepR
		Geom	GEO_WepR_Sonic_Rifle_01
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 80 80 .6
	End
End

End



