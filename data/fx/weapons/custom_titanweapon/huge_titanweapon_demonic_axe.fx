#############################################################
## Costume Anim Entity - Pulse Rifle - 01
#############################################################

FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			Type Local
			At WepL
		Bhvr	WEAPONS\Custom_TitanWeapon\Huge_WepLScale.bhvr
			Anim CustomWeapon_TitanWeapon_Demonic_Axe
		End

	End
End

