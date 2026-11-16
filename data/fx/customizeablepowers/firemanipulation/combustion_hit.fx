#############################################################
## Combustion_Hit.fx
#############################################################

FxInfo
LifeSpan 100

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Posit
		At	Hips
		Part1	CustomizeablePowers\FireManipulation\FireBallTargetSteam.part
		Part2	CustomizeablePowers\FireManipulation\FireBallTargetFlame2.part
		Part3	CustomizeablePowers\FireManipulation\FireBallSpark.part
	End

	Event
		Type	Local
		At	chest
		Part1	CustomizeablePowers\FireManipulation\InfernoCare.part
		Part2	CustomizeablePowers\FireManipulation\Inferno.part
		Lifespan 9
	End

	Event
		Type	Local
		At	chest
		Part1	CustomizeablePowers\FireManipulation\InfernoExplosion.part
		Lifespan 3
	End
End

#############################################################

Condition
	On 	time
	Time 	0
	Repeat  5

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX	CustomizeablePowers\FireManipulation\Child_FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	time
	Time 	0
	Repeat  8

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireManipulation\Child_FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireManipulation\Child_FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

#############################################################

End