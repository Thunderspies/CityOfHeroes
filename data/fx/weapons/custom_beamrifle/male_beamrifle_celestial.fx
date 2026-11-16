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
			PositionOffset	00 0 -0.025
		End
		Anim	CustomWeapon_BeamRifle_Celestial01
	End
End

#############################################################

End