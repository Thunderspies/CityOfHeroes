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
		Sound RES_PowerCannon_Equip 100 100 1
		Bhvr	WEAPONS\Custom_AssaultRifle\Female_WepRScale.bhvr
		BhvrOverride
			PYRRotate		0 -10 0
		End
		Anim	CustomWeapon_AssaultRifle_ResistanceCannon01
	End
End

#############################################################

End