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
		Anim	CustomWeapon_TitanWeapon_Concrete_Mallet
		BhvrOverride
			PositionOffset	-0.1 0 0.7
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
		ChildFX WEAPONS\Custom_TitanWeapon\FX_Concrete_Mallet.fx
	End
End

End
