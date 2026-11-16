#############################################################
## AnimatedTail_Bioluminate.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress UseSkinTint

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		Bhvr	:Female_Scale.bhvr
		// BhvrOverride
		// 	PositionOffset		0 0 0.08
		// End
		Anim	FX_PlayerTail_Bioluminate01
	End
End

#############################################################

End