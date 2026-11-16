#########################################################
## Rikti Portal Stabilizer - Small Lighting Arc
########################################################

FxInfo

#########################################################

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
		Type	Local
		At	Origin
		Part1	:SmallBeamFlare01.part
		Part1	:SmallBeamFlare02.part
	End		
End

########################################################

Condition
	On 	Cycle
	Time 	3

	Event
		Type	Local
		At	Origin
		Part1	:SmallBeam01.part
		Part1	:SmallBeam02.part
		Part1	:SmallBeamGlow1.part
		POther	StartOffset
		Lifespan	3
	End		
End

Condition
	On 	Cycle
	Time 	5
	Chance	0.125

	Event
		Type	Local
		At	Origin
		Part1	:SmallBeam03.part
		POther	StartOffset
		Lifespan	5
	End		
End

Condition
	On 	Cycle
	Time 	5
	Chance	0.125

	Event
		Type	Local
		At	Origin
		Part1	:SmallBeam04.part
		POther	StartOffset
		Lifespan	5
	End		
End

########################################################

End