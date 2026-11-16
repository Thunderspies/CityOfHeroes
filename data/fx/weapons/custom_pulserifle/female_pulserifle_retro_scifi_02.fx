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
		Anim	CustomWeapon_PulseRifle_Retro_SciFi_02
		BhvrOverride
			PositionOffset	-0.010  -0      0.03
			Scale		0.675   0.675   0.675
			PyrRotate 	1       -07     2
	End
End

#############################################################

End