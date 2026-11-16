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
		Bhvr	WEAPONS\Custom_BeamRifle\Huge_WepRScale.bhvr
		Anim	CustomWeapon_BeamRifle_LegacyRifle_01
		BhvrOverride
		#	Scale			1.2 1.2 1.2
		#	PositionOffset		0.2 0 0.03
		End
	End
End


#############################################################

End