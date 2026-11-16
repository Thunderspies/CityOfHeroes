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
		Bhvr	WEAPONS\Custom_Mace\Huge_WepRScale.bhvr
		BhvrOverride
			Scale		1.5 1.5 1.5
			PositionOffset	0.0 0.0 0.5
		End
		Anim	CustomWeapon_Mace_Arachnos_FireAndIce_01
	End
End

#############################################################

End