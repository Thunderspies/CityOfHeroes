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
		BhvrOverride
			PositionOffset	0 0 -0.025
			PyrRotate 	0 -5 -5
		End
		Anim	CustomWeapon_AssaultRifle_CelestialRifle_01
	End
End

#############################################################

End