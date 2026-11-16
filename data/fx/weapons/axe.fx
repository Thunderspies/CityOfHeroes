#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Axe_3
		Sound axeout 70 70 .6
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End