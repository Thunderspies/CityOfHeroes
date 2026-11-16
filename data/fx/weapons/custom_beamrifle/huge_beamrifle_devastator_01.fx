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
		Anim	CustomWeapon_BeamRifle_Devastator_01
		BhvrOverride
			#Scale			1.2 1.2 1.2
			#PositionOffset		0.2 0 0.03
		End
	End
End

Condition
	On Time
	Time 7

	Event
		Type	Local
		At	WepR
		sound	RES_PowerRifle_Equip 100.0 80.0 .5
		LifeSpan 5
	End
End

#############################################################

End