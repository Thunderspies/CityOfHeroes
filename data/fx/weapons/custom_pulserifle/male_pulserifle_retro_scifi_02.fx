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
	End
End

#############################################################

End