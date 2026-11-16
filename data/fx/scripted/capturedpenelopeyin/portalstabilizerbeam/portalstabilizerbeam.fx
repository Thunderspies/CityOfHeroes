#########################################################
## Rikti Portal Stabilizer - Large Beam
########################################################

FxInfo

#LifeSpan	90

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	StartOffset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 16 0
		End
	End

	Event
		EName	TargetOffset
		Type	Local
		At	T_Waist
		LookAt	T_Waist
	End
End

########################################################

Condition
	On 	Cycle
	Time 	6

	Event
		Type	Local
		At	StartOffset
		Part1	:LargeBeam01.part
		Part1	:LargeBeam02.part
		Part1	:LargeBeam_Glow1.part
		POther	TargetOffset
		Lifespan	1
	End		
End

Condition
	On 	Cycle
	Time 	12
	Chance	0.25

	Event
		Type	Local
		At	StartOffset
		Part1	:LargeBeam03.part
		POther	TargetOffset
		Lifespan	1
	End		
End

Condition
	On 	Cycle
	Time 	12
	Chance	0.25

	Event
		Type	Local
		At	StartOffset
		Part1	:LargeBeam04.part
		POther	TargetOffset
		Lifespan	1
	End		
End

########################################################

End