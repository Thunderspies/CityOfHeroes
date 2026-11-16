#############################################################
## Costume Anim Entity - Broadsword - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

	Condition
		On 	Time
		Time 	0

		Event
			Type	Local
			At	WepR
			Anim	CustomWeapon_Broadsword_PostApoc_01
			BhvrOverride
				Scale		0.6 0.6 0.6
				PositionOffset	.04 0 0
				PyrRotate	0 0 0
			End
		End
	End

	// Condition
	// 	On		TRIGGERBITS
	// 	Triggerbits	RIGHTHAND
    // 
	// 	Event
	// 		Ename	SMOKE
	// 		Type 	local
	// 		At	WepR
	// 		While	RIGHTHAND
	// 		Flags	ONEATATIME
	// 		BhvrOverride
	// 			PyrRotate	0 0 180
	// 			PositionOffset 0 0 0.7
	// 		End
	// 		ChildFX	WEAPONS\Custom_Broadsword\CHILD_Fem_PostApoc_Pruner_01.fx
	// 	End
	// End

#############################################################

End