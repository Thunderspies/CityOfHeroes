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
		Until 	AIR
		Flags	OneAtATime
		ChildFX	:Fire_Trail_NOFLY.fx
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
		Until 	AIR
		Flags	OneAtATime
		ChildFX	:Fire_Trail_NOFLY.fx
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
		Until 	AIR
		Flags	OneAtATime
		ChildFX	:Fire_Trail_NOFLY.fx
	End
End
#############################################################

End