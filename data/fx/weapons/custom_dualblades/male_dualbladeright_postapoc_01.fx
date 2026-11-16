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
	// 		At	WepR
	// 		While	DUALHAND
	// 		Flags	ONEATATIME
	// 		BhvrOverride
	// 			PyrRotate	0 0 180
	// 			PositionOffset 0 0 0.9
	// 		End
	// 		ChildFX	WEAPONS\Custom_Broadsword\CHILD_PostApoc_Pruner_01.fx
	// 	End
	// End


#############################################################

End