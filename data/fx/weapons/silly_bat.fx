#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_SillyBaseBallBat
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#############################################################

End