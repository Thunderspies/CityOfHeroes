#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Sound thermite_loop 60 60 .46
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Rikti_Gun_05
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#############################################################

End