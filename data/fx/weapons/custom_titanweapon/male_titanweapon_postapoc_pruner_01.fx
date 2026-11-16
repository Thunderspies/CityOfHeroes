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
			// BhvrOverride
			// 	PyrRotate 0 0 180
			// 	PositionOffset -0.08 -0.02 0
			// End
			Anim	CustomWeapon_TitanWeapon_PostApoc_Pruner_01
		End
	End

	// Condition
	//         On		Triggerbits
	//         Triggerbits	2HLARGE #WEAPON works too
    //     	DoMany		1
    // 
	// 	Event
	// 		Ename	SMOKE
	// 		Type 	local
	// 		At	WepL
	// 		Bhvr	Behaviors\GenericParticleFade.bhvr
	// 		BhvrOverride
	// 			PositionOffset -0.08 -0.02 1.2
	// 		End
	//                 While   2HLARGE #WEAPON
	//                 Flags   OneAtATime
	// 		ChildFX	:CHILD_PostApoc_Pruner_01.fx
	// 	End
	// End


#############################################################

End