#########################################################
## Vanguard Staff of the Magi
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Vanguard
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#########################################################

End