#############################################################
## FlaresTarget.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 80

## PHYSICS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	hips
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		125
			PhysForcePowerJitter	10
		End
		Lifespan 1
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	hips
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		25
			PhysForcePowerJitter	10
		End
		Lifespan 30
	End
End

## HIT 1 ###########################################################

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
		Sound FireExplo 100.0 100.0 .33
	End

	Event
		Type	Local
		At	hips
		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part
		Lifespan 9
	End

	Event
		Type	Local
		At	hips
		Part1	:InfernoExplosion.part
		Lifespan 3
	End
End

Condition
	On 	time
	Time 	1
	Repeat 	8

	Event
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 60
		Lifespanjitter 	20
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	hips
		Part1	:InfernorockBits.part
		Part2	:InfernorockBitz.part
		Lifespan 1
	End
End

## HIT 2 ###########################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Posit
		At	hips
		Part3	:FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		Type	Local
		At	hips
		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part
		Lifespan 9
	End

	Event
		Type	Local
		At	hips
		Part1	:InfernoExplosion.part
		Lifespan 3
	End
End

## HIT 3 ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Posit
		At	hips
		Part3	:FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		Type	Local
		At	hips
		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part
		Lifespan 9
	End

	Event
		Type	Local
		At	hips
		Part1	:InfernoExplosion.part
		Lifespan 3
	End
End

Condition
	On 	time
	Time 	11
	Repeat 	2

	Event
		HardwareOnly
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 60
		Lifespanjitter 	20
	End
End

Condition
	On 	Time
	Time 	13

	Event
		Type	Local
		At	hips
		Part1	:InfernorockBits.part
		Part2	:InfernorockBitz.part
		Lifespan 1
	End
End

## HIT 4 ###########################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Posit
		At	hips
		Part3	:FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		Type	Local
		At	hips
		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part
		Lifespan 9
	End

	Event
		Type	Local
		At	hips
		Part1	:InfernoExplosion.part
		Lifespan 3
	End
End

Condition
	On 	time
	Time 	3
	Repeat 	8

	Event
		HardwareOnly
		Type 	Local
		At 	hips
		ChildFX :Physics_Sparks.fx
		Lifespan 60
		Lifespanjitter 	20
	End
End

## HIT 5 ###########################################################

#Condition
#	On 	Time
#	Time 	20
#
#	Event
#		Type	Posit
#		At	hips
#		Part3	:FireBallSpark.part
#		Sound	FireExplo 100.0 60.0 .8
#	End
#
#	Event
#		Type	Local
#		At	hips
#		Part1	Generic\InfernoCare.part
#		Part2	Generic\Inferno.part
#		Lifespan 9
#	End
#
#	Event
#		Type	Local
#		At	hips
#		Part1	:InfernoExplosion.part
#		Lifespan 3
#	End
#End
#
#Condition
#	On 	Time
#	Time 	30
#
#	Event
#		Type	Local
#		At	hips
#		Part1	:InfernorockBits.part
#		Part2	:InfernorockBitz.part
#		Lifespan 1
#	End
#End

#############################################################

End