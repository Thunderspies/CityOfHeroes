#############################################################
## FireBallHitNO_RING.fx
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
		EName 	Hook
		Type	Local
		At	root
		Geom	RootToChestAdjustment
	End

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
	End

	Event
		EName 	Prime1
		Type	Local
		At	chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		150
			PhysForcePowerJitter	15
		End
		Part1	:InfernoCare.part
		Part2	:Inferno.part
		Sound telekinetic3 80 80 .9
		Lifespan 1
	End

	Event
		EName 	Prime2
		Type	Local
		At	chest
		Part1	:InfernoExplosion.part
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local
		At	chest
		Part1	:InfernorockBits.part
		Part2	:InfernorockBitz.part
		Sound FireExplo 100.0 100.0 .8
		Lifespan 4
	End
End

#############################################################

Condition
	On 	time
	Time 	0
	Repeat  6

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX :FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
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
	Repeat  8

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX :FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

#############################################################

End