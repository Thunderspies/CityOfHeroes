#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
#LifeSpan 50

#############################################################

Input
	InpName Hips
End

#############################################################


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
		Part	:Toxic_Bubbles_Floating.part
		CHILDFX	:Child_Bubbles_AIR_MOVEMENT2.fx
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
		Part	:Toxic_Bubbles_Floating.part
		CHILDFX	:Child_Bubbles_AIR_MOVEMENT2.fx
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
		Part	:Toxic_Bubbles_Floating.part
		CHILDFX	:Child_Bubbles_AIR_MOVEMENT2.fx
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
		Part	:Toxic_Bubbles_Floating.part
		CHILDFX	:Child_Bubbles_AIR_MOVEMENT2.fx
	End
End
#############################################################

End