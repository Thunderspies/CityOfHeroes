#
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			Anim CustomWeapon_TitanWeapon_Railroad_Pike
			At WepL
			Type Local
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
		ChildFX WEAPONS\Custom_TitanWeapon\FX_Railroad_Pike.fx
	End
End

End

