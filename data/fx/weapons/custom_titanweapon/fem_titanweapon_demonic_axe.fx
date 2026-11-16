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
			Bhvr	WEAPONS\Custom_TitanWeapon\Female_WepLScale.bhvr
			Anim CustomWeapon_TitanWeapon_Demonic_Axe
         		BhvrOverride
         			Scale		0.85 0.85 0.85
         			PositionOffset	-0.05 0 0
         			#PyrRotate	-3 3 0
         		End
		End

	End
End


