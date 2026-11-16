#############################################################
## Costume Anim Entity - Left Blade - 04
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

	Condition
		On 	Time
		Time 	0

		Event
			Type	Local
			At	WepL
			Anim	CustomWeapon_BladeLeft_PostApoc_01
			BhvrOverride
				Scale		.8 .8 .8
				PositionOffset	0 0 -.2
				PyrRotate	0 0 0
			End
		End
	End

	// Condition
	// 	On		TRIGGERBITS
	// 	Triggerbits	DUALHAND
    // 
	// 	Event
	// 		Ename	SMOKE
	// 		Type 	local
	// 		At	WepL
	// 		While	DUALHAND
	// 		Flags	ONEATATIME
	// 		BhvrOverride
	// 			PositionOffset 0 0 0.9
	// 		End
	// 		ChildFX	WEAPONS\Custom_Broadsword\CHILD_PostApoc_Pruner_01.fx
	// 	End
	// End

#############################################################

End