#############################################################
## Costume Anim Entity - Right Blade - 05
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
			Bhvr	behaviors\Reverse_Wep.bhvr
			Anim	CustomWeapon_BladeRight_PostApoc_01
			BhvrOverride
				Scale		0.75 0.75 0.75
				PositionOffset	.09 -.03 0
				PyrRotate	0 10 0
			End
		End
	End

	// Condition
	// 	On		TRIGGERBITS
	// 	Triggerbits	WEAPON
    // 
	// 	Event
	// 		Ename	SMOKE
	// 		Type 	local
	// 		At	WepR
	// 		While	WEAPON
	// 		Flags	ONEATATIME
	// 		BhvrOverride
	// 			PyrRotate	0 0 180
	// 			PositionOffset 0.2 0 0.9
	// 		End
	// 		ChildFX	WEAPONS\Custom_Broadsword\CHILD_PostApoc_Pruner_01.fx
	// 	End
	// End

#############################################################

End