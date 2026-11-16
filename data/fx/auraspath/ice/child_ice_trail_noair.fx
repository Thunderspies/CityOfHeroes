#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		TRIGGERBITS
	TRIGGERBITS     FORWARD
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:CHILD_Ice_Trail_NOFLY.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS     BACKWARD
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:CHILD_Ice_Trail_NOFLY.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS     STEPLEFT
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:CHILD_Ice_Trail_NOFLY.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS     STEPRIGHT
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		UNTIL 	AIR
		Flags	OneAtATime
		ChildFX	:CHILD_Ice_Trail_NOFLY.fx
	End

End


#########################################################

End