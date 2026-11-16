#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	WepR
		Geom	GEO_WepR_Mastermind_Military_Rifle_Silencer
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 40 40 .3
	End
End

End