#############################################################
## AnimatedTail_Demon.fx
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
		// BhvrOverride
		// 	PositionOffset		0 0 0.04
		// End
		Anim	FX_PlayerTail_Demon01
	End
End

#############################################################

End