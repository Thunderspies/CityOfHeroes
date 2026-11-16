#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     FORWARD
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:Roach_Trail_CHILD2.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     BACKWARD
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:Roach_Trail_CHILD2.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     STEPLEFT
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:Roach_Trail_CHILD2.fx
	End
End

Condition

	On 		TRIGGERBITS #Cycle
	TRIGGERBITS     STEPRIGHT
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:Roach_Trail_CHILD2.fx
	End

End


#########################################################

End