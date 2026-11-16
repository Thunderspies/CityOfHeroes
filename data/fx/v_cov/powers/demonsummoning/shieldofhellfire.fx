#############################################################
## AbyssalMending_Hit.fx
#############################################################

FxInfo

#############################################################

Condition
	On	TriggerBits
	TriggerBits	NONHUMAN

	Event
		Type	Destroy
		EName	ALL
	End
End

#############################################################

Condition
	On	Cycle
	Time	22
	Chance	0.5
	Event
		EName	SpinnerCW
		Type	PositOnly
		At	Root
		BhvrOverride
			Spin		0 0.3 0
		End
		Lifespan 19
	End

	Event
		EName	Offset
		Type	Local
		At	SpinnerCW
		BhvrOverride
			InitialVelocity		0 .3 0
			PositionOffset		0 0.0 1.5
			StartJitter		2 0.0 1
		End
		Part	:Offset_Fire_Trail.part
		Part	:Offset_Fire_Trail.part
		Part	:Offset_Fire_Highlight_Trail.part
		Part	:Offset_Core.part
		Part	:Offset_Sparks_Trail.part
		Lifespan 	19
		LifespanJitter 	5
	End
End

Condition
	On	Cycle
	Time	22
	Chance	0.5

	Event
		EName	SpinnerCCW
		Type	PositOnly
		At	Root
		BhvrOverride
			Spin	0 -0.3 0
		End
		Lifespan 19
	End

	Event
		EName	Offset2
		Type	Local
		At	SpinnerCCW
		BhvrOverride
			InitialVelocity		0 .3 0
			PositionOffset		0 0.0 1.5
			StartJitter		2 0.0 1
		End
		Part	:Offset_Fire_Trail.part
		Part	:Offset_Fire_Trail.part
		Part	:Offset_Fire_Highlight_Trail.part
		Part	:Offset_Core.part
		Part	:Offset_Sparks_Trail.part
		Lifespan 	19
		LifespanJitter 	5

	End
End

#############################################################

End


