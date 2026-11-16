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
		Bhvr	WEAPONS\Custom_TitanWeapon\Huge_WepLScale.bhvr
		Anim	CustomWeapon_TitanWeapon_SuperPack2_Sword_01
		BhvrOverride
			Scale		1.2 1.2 1.2
			PositionOffset	-0.09 0 -0.15
#			PyrRotate	-1 1 0
		End
	End
End

#############################################################

End