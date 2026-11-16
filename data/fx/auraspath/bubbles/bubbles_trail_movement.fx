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
	ON		TRIGGERBITS
	TRIGGERBITS	JUMP
	DoMany		1

	Event
		Ename	FlyBubbles
		Type	StartPositOnly
		At	Root
		WHILE	JUMP
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 2.0 0.0
			StartJitter	2.0 0.0 2.0
		End
		Part	:Toxic_Bubbles_Floating.part
		ChildFX	:Child_Bubbles_AIR_MOVEMENT2.fx
	End
End

#Condition
#	ON		TRIGGERBITS
#	TRIGGERBITS	FLY
#	DoMany		1
#
#	Event
#		Ename	FlyBubbles
#		Type	StartPositOnly
#		At	Root
#		WHILE	FLY
#		Flags	OneAtATime
#		BhvrOverride
#			PositionOffset	0.0 -1.0 0.0
#			StartJitter	2.0 0.0 2.0
#		End
#		ChildFX	:Child_Bubbles_AIR_MOVEMENT.fx
#	End
#End
#Condition
#	ON		TRIGGERBITS
#	TRIGGERBITS	AIRw
#	DoMany		1
#
#	Event
#		Ename	FlyBubbles
#		Type	StartPositOnly
#		At	Root
#		WHILE	AIR
#		Flags	OneAtATime
#		BhvrOverride
#			PositionOffset	0.0 -1.0 0.0
#			StartJitter	2.0 0.0 2.0
#		End
#		ChildFX	:Child_Bubbles_AIR_MOVEMENT.fx
#	End
#Endw

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

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPLEFT
	DoMany		1

	Event
		Ename	AirBubbles
		Type	Local
		At	Root
		While	STEPLEFT
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
		End
		Part	:Toxic_Bubbles_Floating.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPRIGHT
	DoMany		1

	Event
		Ename	AirBubbles
		Type	Local
		At	Root
		While	STEPRIGHT
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
	TRIGGERBITS	FORWARD
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	PositOnly
		At	Root
		While	FORWARD
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoFLY.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	BACKWARD
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	PositOnly
		At	Root
		While	BACKWARD
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoFLY.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPLEFT
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	PositOnly
		At	Root
		While	STEPLEFT
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoFLY.fx
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPRIGHT
	DoMany		1

	Event
		Ename	GroundBubbles
		Type	PositOnly
		At	Root
		While	STEPRIGHT
		Flags	OneAtATime
		CHILDFX	:CHILD_Bubbles_NoFLY.fx
	End
End

#############################################################

End