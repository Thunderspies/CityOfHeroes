#############################################################
## Costume Anim Entity - Pulse Rifle - 01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_TitanWeapon\Female_WepLScale.bhvr
		Anim	CustomWeapon_TitanWeapon_SuperPack2_Sword_01
		BhvrOverride
			Scale		0.8 0.8 0.8
			PositionOffset	-0.05 0 -0.15
#			PyrRotate	-1 1 0
		End
	End
End

#############################################################

End