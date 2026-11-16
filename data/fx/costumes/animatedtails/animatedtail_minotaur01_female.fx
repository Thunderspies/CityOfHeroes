#############################################################
## AnimatedTail_Minotaur.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		Bhvr	:Female_Scale.bhvr
		// BhvrOverride
		// 	PositionOffset		0 -0.04 0.03
		// End
		Anim	FX_PlayerTail_Minotaur01
	End
End

#############################################################

End