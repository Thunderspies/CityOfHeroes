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
		EName	StabilizerOffset
		Type	Local
		At	Root
		#LookAt	T_Root
		BhvrOverride
			PositionOffset	0 16 0
		End
	End

	Event
		EName	PortalOffset
		Type	Local
		At	T_Root
		#LookAt	Root
		BhvrOverride
			PositionOffset	0 14 0
		End
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	StabilizerOffset
		Sound HamidonElecBlast 80 80 .74
		Part1	:LargeBeamFlare01.part
		Part1	:LargeBeamFlare02.part
	End		
End

########################################################

Condition
	On 	Cycle
	Time 	6

	Event
		Type	Local
		At	PortalOffset
		Part1	:LargeBeam01.part
		Part1	:LargeBeam02.part
		Part1	:LargeBeam_Glow1.part
		POther	StabilizerOffset
		Lifespan	1
	End		
End

Condition
	On 	Cycle
	Time 	12
	Chance	0.25

	Event
		Type	Local
		At	PortalOffset
		Part1	:LargeBeam03.part
		POther	StabilizerOffset
		Lifespan	1
	End		
End

Condition
	On 	Cycle
	Time 	12
	Chance	0.25

	Event
		Type	Local
		At	PortalOffset
		Part1	:LargeBeam04.part
		POther	StabilizerOffset
		Lifespan	1
	End		
End

########################################################

End