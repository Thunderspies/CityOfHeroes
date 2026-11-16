#########################################################
## Blue Mitochondria - Chain Paralyze (Reverse Hit Effect)
########################################################

FxInfo

LifeSpan	90

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ChainTarget
		Type	Posit
		At	Waist
		LookAt	T_Waist
	End

	Event
		Type	Local
		At	ChainTarget
		Sound HamidonElecBlast 80 80 .74
		Part1	:Chain_BeamFlare01.part
		Part1	:Chain_BeamFlare02.part
	End		
End

Condition
	On 	Time
	Time 	0

	Event
		EName	ChainOrigin
		Type	Posit
		At	T_Waist
		LookAt	Waist
	End

	Event
		Type	Local
		At	ChainOrigin
		Sound HamidonElecBlast 80 80 .74
		Part1	:Chain_BeamFlare01.part
		Part1	:Chain_BeamFlare02.part
	End		
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	ChainOrigin
		Part1	:Chain_Beam01.part
		Part1	:Chain_Beam02.part
		Part1	:Chain_BeamGlow2.part
		POther	ChainTarget
	End		
End

########################################################

Condition
	On 	Cycle
	Time 	6

	Event
		Type	Local
		At	ChainOrigin
		Part1	:Chain_Beam04.part
		Part1	:Chain_Beam06.part
		Part1	:Chain_BeamGlow2.part
		POther	ChainTarget
		Lifespan	15
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	ChainOrigin
		Part1	:Chain_Beam04.part
		Part1	:Chain_Beam06.part
		Part1	:Chain_BeamGlow2.part
		POther	ChainTarget
		Lifespan	15
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	ChainOrigin
		Part1	:Chain_Beam01.part
		Part1	:Chain_Beam02.part
		Part2	:Chain_BeamGlow2.part
		POther	ChainTarget
		Lifespan	15
	End		
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.25

	Event
		Type	Local
		At	ChainOrigin
		Part1	:Chain_Beam03.part
		Part1	:Chain_Beam05.part
		Part1	:Chain_BeamGlow1.part
		POther	ChainTarget
		Lifespan	15
	End		
End

########################################################

End