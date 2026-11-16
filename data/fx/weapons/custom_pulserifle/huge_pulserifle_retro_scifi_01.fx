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
		Bhvr	WEAPONS\Custom_AssaultRifle\Female_WepRScale.bhvr
		Anim	CustomWeapon_PulseRifle_Retro_SciFi_01
		BhvrOverride
			PositionOffset	-0.1  -0      0.03
			Scale		1   1   1
			PyrRotate 	0       -07     -1
	End
End

#############################################################

End