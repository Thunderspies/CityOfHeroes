#############################################################
## Costume Anim Entity - Pulse Rifle - 01
#############################################################

FxInfo

Flags InheritAlpha ##InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_PulseRifle\Female_WepRScale.bhvr
		Anim	CustomWeapon_PulseRifle_03
	End
End

#############################################################

End