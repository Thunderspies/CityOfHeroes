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
		ChildFx :N_Rain_Splash_Child3.fx
		EName Splashes
		Flags	OneAtATime
		Until AIR
	End

End

Condition
	On TriggerBits
	TriggerBits FEMALE
	DoMany 	1

	Event
		At Root
		Type Local
		ChildFx :N_Rain_Splash_Child3.fx
		EName Splashes
		Flags	OneAtATime
		Until AIR
	End

End

Condition
	On TriggerBits
	TriggerBits HUGE
	DoMany 	1

	Event
		At Root
		Type Local
		ChildFx :N_Rain_Splash_Child3.fx
		EName Splashes
		Flags	OneAtATime
		Until AIR
	End

End

End
