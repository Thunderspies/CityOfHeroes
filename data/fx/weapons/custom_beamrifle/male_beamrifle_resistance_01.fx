#############################################################
## Costume Anim Entity - Assault Rifle - Base
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
		Anim	CustomWeapon_BeamRifle_Resistance_01
		BhvrOverride
			Scale			1 1 1
			PositionOffset		0.12 0 0
		End
	End
End

Condition
	On Time
	Time 7

	Event
		Type	Local
		At	WepR
		sound	RES_PowerRifle_Equip 100.0 80.0 .5
		LifeSpan 5
	End
End

#############################################################

End