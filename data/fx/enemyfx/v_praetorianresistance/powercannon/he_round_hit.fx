#############################################################
## FireBallHit.fx
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
		At	hips
		Sound RES_PowerCannon_HitHERound 50 50 .75
		Part1	CustomizeablePowers\FireBlast\FireBallTargetSteam.part
		Part2	CustomizeablePowers\FireBlast\FireBallTargetFlame2.part
		Part3	CustomizeablePowers\FireBlast\FireBallSpark.part
	End

	Event
		Type	Local
		At	chest
		Part1	CustomizeablePowers\FireBlast\InfernoCare.part
		Part2	CustomizeablePowers\FireBlast\Inferno.part
		Lifespan 9
	End

	Event
		Type	Local
		At	chest
		Part1	CustomizeablePowers\FireBlast\InfernoExplosion.part
		Lifespan 2
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat  5

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireBlast\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	Time
	Time 	0
	Repeat  8

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireBlast\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX CustomizeablePowers\FireBlast\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

#############################################################

End