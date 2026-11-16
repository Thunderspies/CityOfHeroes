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
		UNTIL 	FLY
		Flags	OneAtATime
		ChildFX	:GRANDCHILD_Ice_Trail_MOVEMENT.fx
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
		UNTIL 	FLY
		Flags	OneAtATime
		ChildFX	:GRANDCHILD_Ice_Trail_MOVEMENT.fx
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
		UNTIL 	FLY
		Flags	OneAtATime
		ChildFX	:GRANDCHILD_Ice_Trail_MOVEMENT.fx
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
		UNTIL 	FLY
		Flags	OneAtATime
		ChildFX	:GRANDCHILD_Ice_Trail_MOVEMENT.fx
	End

End


#########################################################

End