#############################################################
## Costume Anim Entity - Mace - Base
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
		Bhvr	WEAPONS\Custom_Mace\Female_WepRScale.bhvr
		Anim	CustomWeapon_Celestial_Mace_01
	End
End

#############################################################

End