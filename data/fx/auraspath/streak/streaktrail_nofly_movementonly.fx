#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#############################################################

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     MALE
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Until 	FLY
		Flags	OneAtATime
		ChildFX	:StreakTrail_NOAIR_MovementOnly.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     FEMALE
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Until 	FLY
		Flags	OneAtATime
		ChildFX	:StreakTrail_NOAIR_MovementOnly.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     HUGE
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Until 	FLY
		Flags	OneAtATime
		ChildFX	:StreakTrail_NOAIR_MovementOnly.fx
	End
End

#############################################################

End