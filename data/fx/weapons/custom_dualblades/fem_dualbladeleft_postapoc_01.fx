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
			BhvrOverride
				Scale		0.6 0.6 0.6
				PYRRotate	0 0 0
				PositionOffset	.1 0 -.2
			End
			Bhvr	behaviors\Reverse_Wep.bhvr
			Anim	CustomWeapon_BladeRight_PostApoc_01
		End
	End

	// Condition
	// 	On		TRIGGERBITS
	// 	Triggerbits	WEAPON
    // 
	// 	Event
	// 		Ename	SMOKE
	// 		Type 	local
	// 		At	WepL
	// 		While	WEAPON
	// 		Flags	ONEATATIME
	// 		BhvrOverride
	// 			PositionOffset 0 0 0.65
	// 		End
	// 		ChildFX	WEAPONS\Custom_Broadsword\CHILD_Fem_PostApoc_Pruner_01.fx
	// 	End
	// End

#############################################################

End