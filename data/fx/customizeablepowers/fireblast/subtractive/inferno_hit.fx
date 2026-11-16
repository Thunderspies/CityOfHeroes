#############################################################
## FireBallTarget1.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 80

#############################################################

Condition
	On 	time
	Time 	0
	Repeat  8

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
	On 	time
	Time 	0
	Repeat  12

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX :FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		EName 	Prime1
		Type	Local
		At	hips
		Part1	:InfernoCare.part
		Part2	:Inferno.part
		Lifespan 9
	End

	Event
		EName 	Prime2
		Type	Local
		At	hips
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	10
		End
		Part1	:InfernoExplosion.part
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local
		At	hips
		Part1	:InfernorockBits.part
		Part2	:InfernorockBitz.part
		Lifespan 4
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