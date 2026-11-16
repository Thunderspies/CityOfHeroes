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
		UNTIL 	FLY
		Flags	OneAtATime
		ChildFX	:RadiantTrail_CHILD.fx
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
		UNTIL 	FLY
		Flags	OneAtATime
		ChildFX	:RadiantTrail_CHILD.fx
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
		UNTIL 	FLY
		Flags	OneAtATime
		ChildFX	:RadiantTrail_CHILD.fx
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
		UNTIL 	FLY
		Flags	OneAtATime
		ChildFX	:RadiantTrail_CHILD.fx
	End

End


#########################################################

End