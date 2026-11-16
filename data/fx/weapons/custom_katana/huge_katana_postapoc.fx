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
			Anim	CustomWeapon_Katana_PostApoc
			BhvrOverride
				Scale		.9 .9 .9
				PositionOffset	0 0 -.4
				PYRRotate	0.0 0.0 0.0
			End
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