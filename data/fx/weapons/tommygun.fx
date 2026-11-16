#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	4

	Event
		EName 	Pri
		Type	Local
		At	WepR
		Geom	GEO_WepR_TommyGun_01
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 50 50 .3
	End
End

End



