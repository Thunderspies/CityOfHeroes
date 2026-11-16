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
		BhvrOverride
			Scale		1.0 1.0 1.0
			PositionOffset	0.0 0.0 0.3
		End
		Anim	CustomWeapon_Mace_Arachnos_FireAndIce_01
	End
End

#############################################################

End