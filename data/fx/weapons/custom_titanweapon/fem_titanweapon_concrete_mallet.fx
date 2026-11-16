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
		BhvrOverride
			Scale		0.80 0.9 0.80
			PositionOffset	-0.1 0 0.5
			PyrRotate	-2 -1 0
		End
		Anim	CustomWeapon_TitanWeapon_Concrete_Mallet
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
		ChildFX WEAPONS\Custom_TitanWeapon\FX_Concrete_Mallet.fx
	End
End

End


