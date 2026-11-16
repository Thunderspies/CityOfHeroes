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
		CHILDFX	:CHILD_RainbowTrail_MOVEMENT.fx
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
		CHILDFX	:CHILD_RainbowTrail_MOVEMENT.fx
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
		CHILDFX	:CHILD_RainbowTrail_MOVEMENT.fx
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
		CHILDFX	:CHILD_RainbowTrail_MOVEMENT.fx
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
		CHILDFX	:CHILD_RainbowTrail_MOVEMENT.fx
	End
End
#########################################################

End