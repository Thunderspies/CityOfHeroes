#########################################################
##	Door
#########################################################

#########################################################
FxInfo

LifeSpan	3000

#########################################################
Condition

	Event
		Type	start
		At	chest
		ChildFx	GENERIC\ENTITYVANISH.FX

	End

End
#########################################################
Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Chest
		Anim	Door_Lambda_Reinforcement_Deactivated
	End
End
#########################################################
Condition

	Event
		Type	start
		At	chest
		Bhvr	Behaviors/FadeOut.bhvr
	End
#########################################################
End

End

