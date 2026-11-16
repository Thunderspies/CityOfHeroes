#############################################################
## SecondaryMutation_AcuteSenses_Continuing.fx
#############################################################

FxInfo

#############################################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	:SecondaryMutation_AcuteSenses_Hit.fx
		Lifespan 100
	End
End

Condition
	On	Cycle
	Time	150
	Chance	.5

	Event
		Type	Local
		At	Root
		ChildFX	:SecondaryMutation_AcuteSenses_Child.fx
		Lifespan 20
	End
End

#############################################################

End