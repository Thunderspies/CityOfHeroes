#############################################################
## Rain_Splash_Child.fx
#############################################################

FxInfo

###########################################################

Condition
	On TriggerBits
	TriggerBits MALE
	DoMany 	1

	Event
		At Root
		Type Local
		ChildFx :N_Rain_Splash_Child2.fx
		EName Splashes
		Flags	OneAtATime
		Until FLY
	End

End

Condition
	On TriggerBits
	TriggerBits FEMALE
	DoMany 	1

	Event
		At Root
		Type Local
		ChildFx :N_Rain_Splash_Child2.fx
		EName Splashes
		Flags	OneAtATime
		Until FLY
	End

End

Condition
	On TriggerBits
	TriggerBits HUGE
	DoMany 	1

	Event
		At Root
		Type Local
		ChildFx :N_Rain_Splash_Child2.fx
		EName Splashes
		Flags	OneAtATime
		Until FLY
	End

End

End