#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	PositOnly
		At	Root
		Until	AIR
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_Trail.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	PositOnly
		At	Root
		Until	AIR
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_Trail.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	PositOnly
		At	Root
		Until	AIR
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_Trail.fx
	End
End

#############################################################

End