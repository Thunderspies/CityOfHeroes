#############################################################
## Costume Anim Entity - Katana - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

	Condition
		On 	Time
		Time 	5

		Event
			Type	Local
			At	WepL
			BhvrOverride
				Scale		0.75 0.75 0.75
				PYRRotate	0.0 0.0 180.0
				PositionOffset	0 0 -.3
			End
			Anim	CustomWeapon_Katana_PostApoc
		End
	End

	// Condition
	// 	On		TRIGGERBITS
	// 	Triggerbits	LEFTHAND
    // 
	// 	Event
	// 		Ename	SMOKE
	// 		Type 	local
	// 		At	WepL
	// 		While	LEFTHAND
	// 		Flags	ONEATATIME
	// 		Bhvr	Behaviors\GenericParticleFade.bhvr
	// 		BhvrOverride
	// 			PositionOffset 0 0 0.7
	// 		End
	// 		ChildFX	WEAPONS\Custom_Broadsword\CHILD_PostApoc_Pruner_01.fx
	// 	End
	// End

#############################################################

End