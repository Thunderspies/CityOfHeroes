#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On 		TRIGGERBITS
	TRIGGERBITS	JUMP
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		Flags	OneAtATime
		WHILE	JUMP
		CHILDFX	:CHILD_StormCloud_Trail_Movement.fx
	End
End

#########################################################

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	FORWARD
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	FORWARD
		CHILDFX	:CHILD_StormCloud_Trail_Movement.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	BACKWARD
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	BACKWARD
		CHILDFX	:CHILD_StormCloud_Trail_Movement.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	STEPLEFT
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	STEPLEFT
		CHILDFX	:CHILD_StormCloud_Trail_Movement.fx
	End
End

Condition

	On 		TRIGGERBITS
	TRIGGERBITS	STEPRIGHT
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Flags	OneAtATime
		WHILE	STEPRIGHT
		CHILDFX	:CHILD_StormCloud_Trail_Movement.fx
	End
End
#########################################################

End