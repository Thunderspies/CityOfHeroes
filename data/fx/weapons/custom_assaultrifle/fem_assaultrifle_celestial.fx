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
		Bhvr	WEAPONS\Custom_AssaultRifle\Female_WepRScale.bhvr
		Anim	CustomWeapon_AssaultRifle_CelestialRifle_01
		BhvrOverride
			PositionOffset	0.1 -0 -0.05
			PyrRotate 	0 -10 -5
		End
	End
End

#############################################################

End