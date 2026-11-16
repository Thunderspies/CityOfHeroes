#############################################################
## Costume Anim Entity - Pulse Rifle - 01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.1 0.0 -0.2
			Scale	1.4 1.4 1.4
		End
		Anim	CustomWeapon_TitanWeapon_FireAndIce_Mace_01
	End
End

#############################################################

End