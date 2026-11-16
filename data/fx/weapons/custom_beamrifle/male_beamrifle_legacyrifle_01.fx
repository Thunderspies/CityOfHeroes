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
		Anim	CustomWeapon_BeamRifle_LegacyRifle_01
		BhvrOverride
			#Scale			1 1 1
			#PositionOffset		0.2 0 0
		End
	End
End


#############################################################

End