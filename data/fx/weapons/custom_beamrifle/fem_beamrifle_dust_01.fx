#############################################################
## Costume Anim Entity - Pulse Rifle - 01
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
		Anim	CustomWeapon_BeamRifle_DUST_01
		BhvrOverride
			Scale			0.7 0.7 0.7
			PositionOffset		0 0 0
		End
	End
End

#############################################################

End