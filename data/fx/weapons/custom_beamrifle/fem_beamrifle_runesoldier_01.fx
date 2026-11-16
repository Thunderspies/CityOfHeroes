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
		Anim	CustomWeapon_BeamRifle_Runesoldier_01
		BhvrOverride
			Scale			0.8 0.8 0.8
			PositionOffset		0.05 0 0
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