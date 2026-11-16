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
			// Bhvr	WEAPONS\Custom_TitanWeapon\Huge_WepLScale.bhvr
			// BhvrOverride
			// 	PyrRotate 0 0 180
			// 	PositionOffset -0.1 -0.05 0
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
	// 		Type local
	// 		At	WepL
	// 		Bhvr	Behaviors\GenericParticleFade.bhvr
	// 		BhvrOverride
	// 			PositionOffset -0.1 -0.05 1.5
	// 		End
	//                 While   2HLARGE #WEAPON
	//                 Flags   OneAtATime
	// 		ChildFX	:CHILD_PostApoc_Pruner_01.fx
	// 	End
	// End


#############################################################

End