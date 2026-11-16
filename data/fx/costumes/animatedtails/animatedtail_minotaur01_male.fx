#############################################################
## AnimatedTail_Minotaur01.fx
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
		// 	PositionOffset		0 -0.1 -0.03
		// End
		Anim	FX_PlayerTail_Minotaur01
	End
End


#############################################################

End