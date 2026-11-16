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
			PositionOffset	-0.03 0.0 -0.25
			Scale	0.6 1 1
			PyrRotate -0 -5 0
		End
		Anim	CustomWeapon_TitanWeapon_FireAndIce_Mace_01
	End
End

#############################################################

End