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
		Anim	CustomWeapon_BeamRifle_Celestial01
		BhvrOverride
			Scale			1 1 1
			PositionOffset		0.25 0.05 0.03
		End
	End
End

#############################################################

End