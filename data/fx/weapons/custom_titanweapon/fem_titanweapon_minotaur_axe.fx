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
		Anim	CustomWeapon_TitanWeapon_Minotaur_Axe
		BhvrOverride
			Scale		0.95 0.95 0.95
			PositionOffset	-0.1 0 -0.1
			PyrRotate	-2 0 0
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
		ChildFX WEAPONS\Custom_TitanWeapon\FX_Minotaur_Axe.fx
		BhvrOverride
			Scale		0.95 0.95 0.95
			PositionOffset	-0.1 0 -0.1
			PyrRotate	-2 0 0
		End
	End
End
