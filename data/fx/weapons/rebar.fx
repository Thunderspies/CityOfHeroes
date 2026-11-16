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
		Geom	GEO_WepR_Rebar
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound pipeout 55 55 .6

	End
End

End