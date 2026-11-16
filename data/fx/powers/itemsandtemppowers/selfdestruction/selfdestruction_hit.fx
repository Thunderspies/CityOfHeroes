#############################################################
## FireBallTarget1.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat  8

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

#	Event
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	hips
#		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
End

Condition
	On 	Time
	Time 	0
	Repeat  12

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

#	Event
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	hips
#		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	POWERS\FireControl\FireBallTargetSteam.part
		Part2	POWERS\FireControl\FireBallTargetFlame2.part
		Part3	POWERS\FireControl\FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		EName 	Prime1
		Type	Local
		At	hips
		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part
		Lifespan 9
	End

	Event
		EName 	Prime2
		Type	Local
		At	hips
		Part1	POWERS\FireControl\InfernoExplosion.part
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	10
		End
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
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		Lifespan 4
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 20
	End
End

#############################################################

End