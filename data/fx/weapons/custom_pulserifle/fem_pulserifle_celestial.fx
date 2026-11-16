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
		Bhvr	WEAPONS\Custom_PulseRifle\Female_WepRScale.bhvr
		BhvrOverride
			PositionOffset	-0.02 0 -0.15
		End
		Anim	CustomWeapon_PulseRifle_CelestialRifle_01
	End
End

#############################################################

End