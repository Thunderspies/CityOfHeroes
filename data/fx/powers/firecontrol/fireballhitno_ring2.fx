#############################################################
## FireBallHitNO_RING2.fx
#############################################################

FxInfo
LifeSpan	100

#############################################################

Input
	InpName	Hips
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
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

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
		Sound atomicblast3 80 80 .66

	End


	Event
		Ename	ExplosiveForce
		Type	Start
		At	hips

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		200
			PhysForcePowerJitter	25
		End

		Lifespan	1

	End
	Event
		EName 	Prime1
		Type	Local
		At	chest

		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part
		Sound telekinetic3 80 80 .9

	End

	Event
		EName 	Prime2
		Type	Local
		At	chest
		Part1	:InfernoExplosion.part


	End

	Event
		EName 	RadiusFxScale
		Type	start
		At	Hook
		altpiv	1
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		#Part1	powers/firecontrol/FireBallHitRing1.part
		#Part2	powers/firecontrol/FireBallHitRing2.part

	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local
		At	chest
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		Sound FireExplo 100.0 100.0 .8
	End

End

#Condition
#	Time 	3
#	Chance	.8
#
#	Event
#		Type	start
#		At	chest
#
#		Bhvr	Behaviors\StartOffset.bhvr
#		Part1	:BurningMeteor.part
#		Lifespan	130
#	End
#
#End
#
#Condition
#	Time 	3
#	Chance	.8
#
#	Event
#		Type	start
#		At	chest
#
#		Bhvr	Behaviors\StartOffset2.bhvr
#		Part1	:BurningMeteor.part
#		Lifespan	130
#	End
#
#End
#
#Condition
#	Time 	3
#	Chance	.8
#
#	Event
#		Type	start
#		At	chest
#
#		Bhvr	Behaviors\StartOffset3.bhvr
#		Part1	:BurningMeteor.part
#		Lifespan	130
#	End
#
#End
#
#Condition
#	Time 	3
#	Chance	.8
#
#	Event
#		Type	start
#		At	chest
#
#		Bhvr	Behaviors\StartOffset4.bhvr
#		Part1	:BurningMeteor.part
#		Lifespan	130
#	End
#
#End

Condition
	On 	Time
	Time	3
	Repeat  2

	Event
		SoftwareOnly
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstComet.fx
		Lifespan 70
		LifespanJitter 	20
	End

#	Event
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	Chest
#		ChildFX V_COV\PhysicsEnabled\FireBurstComet.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName 	RadiusFxScale
		Type	Destroy
	End


End

Condition
	On 	Time
	Time 	120

	Event
		EName 	All
		Type	Destroy
	End


End


End