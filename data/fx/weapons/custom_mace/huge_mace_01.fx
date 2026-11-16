#############################################################
## Costume Anim Entity - Mace - 01
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
		Bhvr	WEAPONS\Custom_Mace\Huge_WepRScale.bhvr
		Anim	CustomWeapon_Mace_01
	End
End

#############################################################

End