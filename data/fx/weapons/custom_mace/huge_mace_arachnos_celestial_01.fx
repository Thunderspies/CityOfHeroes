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
			PositionOffset	0.1 0 0.2
		End
		Anim	CustomWeapon_Celestial_Arachnos_Mace_01
	End
End

#############################################################

End