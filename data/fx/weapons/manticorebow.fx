#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress IsWeapon

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Geom	manticoreBow
		Sound axeout 70 70 .6
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End