#########################################################
##	jack
##
FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	Arachnos_WristRocket

		Sound claw1b 60 60 .5
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

	Event
		EName 	Prime
		Type	Local
		At	WepL
		Geom	Arachnos_WristRocket_L
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

End

End