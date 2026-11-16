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
			BhvrOverride
				Scale		0.75 0.75 0.75
				PYRRotate	0.0 0.0 180
				PositionOffset	0 0 -.2
			End
			Anim	CustomWeapon_Broadsword_PostApoc_01
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
	// 		While	RIGHTHAND
	// 		Flags	ONEATATIME
	// 		Bhvr	Behaviors\GenericParticleFade.bhvr
	// 		BhvrOverride
	// 			PyrRotate	0 0 180
	// 			PositionOffset 	0 0 0.8
	// 		End
	// 		ChildFX	WEAPONS\Custom_Broadsword\CHILD_PostApoc_Pruner_01.fx
	// 	End
	// End

#############################################################

End