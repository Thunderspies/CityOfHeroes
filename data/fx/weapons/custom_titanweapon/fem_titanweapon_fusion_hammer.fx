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
		Anim	CustomWeapon_TitanWeapon_Fusion_Hammer
		BhvrOverride
			#Scale		0.85 0.85 0.85
			PositionOffset	-0.1 0 -0.1
			PyrRotate	-3 3 0
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
		ChildFX WEAPONS\Custom_TitanWeapon\FX_Fusion_Hammer.fx
		BhvrOverride
			PositionOffset	-0.1 0 -0.1
			PyrRotate	-3 3 0
		End
	End
End

End

