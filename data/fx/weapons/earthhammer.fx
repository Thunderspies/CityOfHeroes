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
		EName 	Prime
		Type	Local
		At	WepR
		Geom	FX_StoneMallet
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#############################################################

End