#
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress


###########################################################
Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Anim	CustomWeapon_TitanWeapon_Minotaur_Axe
		BhvrOverride
			Scale		1.1 1.1 1.1
			PositionOffset	-0.1 0 0
		#	PyrRotate	-2 0 0
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
		ChildFX WEAPONS\Custom_TitanWeapon\FX_Minotaur_Axe.fx
		BhvrOverride
			Scale		1.1 1.1 1.1
			PositionOffset	-0.1 0 0
		#	PyrRotate	-2 0 0
		End
	End
End

End