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
		Type	StartPositOnly
		At	Root
		Until	FLY
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoAIR.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	StartPositOnly
		At	Root
		Until	FLY
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoAIR.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	StartPositOnly
		At	Root
		Until	FLY
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoAIR.fx
	End
End

#############################################################

End