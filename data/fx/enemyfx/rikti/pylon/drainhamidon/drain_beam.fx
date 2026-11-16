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
		EName	PylonOffset
		Type	Local
		At 	Chest
		bhvroverride
			positionoffset	0.0 8.0 0.0
			pyrrotate	0.0 0.0 0.0
		End
	End
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	PylonOffset
		#Sound	HamidonElecBlast 80 70 .74
		Part1	:LargeBeamFlare01.part
		Part1	:LargeBeamFlare02.part
		POther	T_Waist
	End		
End

Condition
	On 	Cycle
	Time 	6

	Event
		Type	Local
		At	PylonOffset
		Part1	:LargeBeam01.part
		Part1	:LargeBeam02.part
		Part1	:LargeBeam_Glow1.part
		POther	T_Waist
		Lifespan	1
	End		
End

Condition
	On 	Cycle
	Time 	12
	Chance	0.25

	Event
		Type	Local
		At	PylonOffset
		Part1	:LargeBeam03.part
		POther	T_Waist
		Lifespan	1
	End		
End

Condition
	On 	Cycle
	Time 	12
	Chance	0.25

	Event
		Type	Local
		At	PylonOffset
		Part1	:LargeBeam04.part
		POther	T_Waist
		Lifespan	1
	End		
End

########################################################

Condition
	On 	Cycle
	Time 	45
	Chance	1

	Event
		Type	Start
		At	T_Waist
		ChildFX	:Drain_Pulse.fx
		Lifespan	60
	End		
End

########################################################

End