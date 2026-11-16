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
		Bhvr	WEAPONS\Custom_BeamRifle\Female_WepRScale.bhvr
		BhvrOverride
			PositionOffset	0.12 0.1 -0.15
		End
		Anim	CustomWeapon_BeamRifle_Celestial01
	End
End

#############################################################

End