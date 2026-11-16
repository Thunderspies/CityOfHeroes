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
		BhvrOverride
			PositionOffset	0 0 0.1
			Scale		1.25 1.25 1.25
		End
		Anim	FX_PlayerTail_Reptile01
	End
End

#############################################################

End