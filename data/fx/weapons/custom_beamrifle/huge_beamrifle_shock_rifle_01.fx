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
		Anim	CustomWeapon_BeamRifle_Shock_Rifle_01
	End
End

#############################################################

End