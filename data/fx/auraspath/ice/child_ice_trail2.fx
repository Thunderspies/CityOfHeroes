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
	#	Part	:FrostCircle.part
		ChildFX	:GRANDCHILD_Ice_Trail.fx
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
		ChildFX	:GRANDCHILD_Ice_Trail.fx
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
		ChildFX	:GRANDCHILD_Ice_Trail.fx
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
		ChildFX	:GRANDCHILD_Ice_Trail.fx
	End

End


#########################################################

End