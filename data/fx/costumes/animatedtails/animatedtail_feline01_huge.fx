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
		Bhvr	:Huge_Scale.bhvr
		// BhvrOverride
		// 	PositionOffset		0 -0.15 -0.07
		// End
		Anim	FX_PlayerTail_Feline01
	End
End

#############################################################

End