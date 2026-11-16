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
		Anim	CustomWeapon_AssaultRifle_Resistance02
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