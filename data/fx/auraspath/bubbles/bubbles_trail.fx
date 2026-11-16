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
	On	Time
	Time	0

	Event

		Type	PositOnly
		At	Root
		Part	:Toxic_Bubbles_Floating_Sparse.part
	End
End

##############################################################

Condition
	ON		TRIGGERBITS
	TRIGGERBITS	FLY
	DoMany		1

	Event
		Ename	FlyBubbles
		Type	StartPositOnly
		At	Root
		WHILE	FLY
		Flags	OneAtATime
		BhvrOverride
			StartJitter	2.0 0.0 2.0
		End
		ChildFX	:Child_Bubbles_FLY.fx
	End
End

Condition
	ON		TRIGGERBITS
	TRIGGERBITS	AIR
	DoMany		1

	Event
		Ename	FlyBubbles
		Type	StartPositOnly
		At	Root
		WHILE	AIR
		Flags	OneAtATime
		BhvrOverride
			StartJitter	2.0 0.0 2.0
		End
		ChildFX	:Child_Bubbles_AIR.fx
	End
End

##############################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FORWARD
	DoMany		1

	Event
		Ename	AirBubbles
		Type	Local
		At	Root
		While	FORWARD
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
		End
		Part	:Toxic_Bubbles_Floating.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	BACKWARD
	DoMany		1

	Event
		Ename	AirBubbles
		Type	Local
		At	Root
		While	BACKWARD
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
		End
		Part	:Toxic_Bubbles_Floating.part
	End
End

##############################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	PositOnly
		At	Root
		While	MALE
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoFLY.fx
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
		While	FEMALE
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoFLY.fx
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
		While	HUGE
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoFLY.fx
	End
End

#############################################################

End