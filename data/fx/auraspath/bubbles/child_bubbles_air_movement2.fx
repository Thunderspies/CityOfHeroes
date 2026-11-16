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
	On		Cycle
	Time		2
	Random		1

	Event
		EName	AIRBubbles
		Type	Start
		At	Root
		BhvrOverride
			InitialVelocity		0.0 0.05 0.0
			InitialVelocityJitter	0.02 0.02 0.02
			PositionOffset		0.0 3.0 0.0
			StartJitter		4.0 2.0 4.0
		End
		CHILDFX	:CHILD_Bubbles_AIR2.fx
		Lifespan 	40
	End


	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
			StartJitter	4.0 0.0 4.0
		End
		ChildFX	:Child_Bubbles_Small.fx
		Lifespan 50
	End
End

#############################################################

End