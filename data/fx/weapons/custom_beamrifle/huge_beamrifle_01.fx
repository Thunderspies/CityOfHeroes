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
		Bhvr	WEAPONS\Custom_BeamRifle\Huge_WepRScale.bhvr
		Anim	CustomWeapon_BeamRifle_VillainBase
	End
End

#############################################################

End