
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
		Geom	GEO_WepR_Sledgehammer_1
		SOund	shotgunout 50 40 .22
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End