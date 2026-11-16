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
		Geom	GEO_WepR_Machete_1
		Sound axeout 60 60 .45
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End