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
		Geom	GEO_WepR_Goldbrickers_SonicRifle
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 80 80 .6
	End
End

End



