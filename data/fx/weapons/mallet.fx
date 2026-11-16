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
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Mallet
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound axeout 60 60 .5
	End
End

End