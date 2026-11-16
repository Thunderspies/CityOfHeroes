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
			// Bhvr	WEAPONS\Custom_TitanWeapon\Female_WepLScale.bhvr
			Anim	CustomWeapon_TitanWeapon_PostApoc_Pruner_01
			BhvrOverride
				// PyrRotate 0 0 180
				PositionOffset -0.01 0 0
			End
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
	// 			PositionOffset -0.08 -0.02 .8
	// 		End
	//                 While   2HLARGE #WEAPON
	//                 Flags   OneAtATime
	// 		ChildFX	WEAPONS\Custom_Broadsword\CHILD_PostApoc_Pruner_01.fx
	// 	End
	// End


#############################################################

End