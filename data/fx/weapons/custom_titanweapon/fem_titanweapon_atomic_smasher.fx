#############################################################
## Costume Anim Entity - Pulse Rifle - 01
#############################################################
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress
###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_TitanWeapon\Female_WepLScale.bhvr
		Anim	CustomWeapon_TitanWeapon_Atomic_Smasher
		BhvrOverride
			PositionOffset	-0.1 0 0
			PyrRotate	0 0 0
		End
	End
End

Condition
        On		Triggerbits
        Triggerbits	2HLARGE #WEAPON works too
        DoMany		1

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	WepL
                While   2HLARGE #WEAPON
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_TitanWeapon\Female_WepLScale.bhvr
		ChildFX WEAPONS\Custom_TitanWeapon\FX_Atomic_Smasher.fx
	End
End

