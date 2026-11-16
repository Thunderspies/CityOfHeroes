#############################################################
## FireBallHit.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Posit
		At	hips
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
	End

	Event
		Type	Local
		At	chest
		Part1	:InfernoCare.part
		Part2	:Inferno.part
		Lifespan 9
	End

	Event
		Type	Local
		At	chest
		Part1	:InfernoExplosion.part
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
		ChildFX :FireBurstOUT.fx
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
		ChildFX :FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX :FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

#############################################################

End