#############################################################
## Costume Anim Entity - Pulse Rifle - 01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Geom	GEO_WepL_TestStaff_01
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#############################################################

End