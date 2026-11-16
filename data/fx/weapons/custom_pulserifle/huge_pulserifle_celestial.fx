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
		Bhvr	WEAPONS\Custom_PulseRifle\Huge_WepRScale.bhvr
		Anim	CustomWeapon_PulseRifle_CelestialRifle_01
	End
End

#############################################################

End