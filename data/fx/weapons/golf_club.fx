#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName 	Prime
		Type	Local
		At	WepL
		Geom	WepL_golfclub
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#############################################################

End