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
		Geom	GEO_WepR_Revolver_3
		Sound shotgunout 70 70 .55
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End