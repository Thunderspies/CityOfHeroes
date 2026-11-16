#########################################################
##	template

FxInfo


Flags InheritAnimScale InheritAlpha

#######  arms  ####################################


Condition
	On 	Time
	Time	0

	Event
		EName 	Arms
		Type	Local
		At	Chest
		Anim	FX_goblin_multi_arms
		Bhvr	Behaviors/arms_offset.bhvr
	End


End


End		