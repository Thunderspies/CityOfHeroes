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
		Bhvr	WEAPONS\Custom_BeamRifle\Female_WepRScale.bhvr
		Anim	CustomWeapon_BeamRifle_LegacyRifle_01
		BhvrOverride
			#Scale			0.8 0.8 0.8
			#PositionOffset		0.05 0 0
		End
	End
End

#############################################################

End