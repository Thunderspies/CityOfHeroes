#############################################################
## SecondaryMutation_PainTolerance_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	:SecondaryMutation_PainTolerance_Hit.fx
		Lifespan 100
	End
End

Condition
	On	Cycle
	Time	120
	Chance	.5

	Event
		Type	Local
		At	Root
		ChildFX	:SecondaryMutation_PainTolerance_Child.fx
		Lifespan 20
	End
End

#############################################################

End