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
End

Condition
	On 	time
	Time 	0
	Repeat  8

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
		#Sound	Atomicblast 100 30 .9

	End

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		#Sound	FireExplo 100.0 30.0 .6
	End


	Event
		EName 	Prime1
		Type	Local
		At	chest

		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part

	End

	Event
		EName 	Prime2
		Type	Local
		At	chest
		Part1	:InfernoExplosion.part


	End

#	Event
#		EName 	RadiusFxScale
#		Type	start
#		At	Hook
#		altpiv	1
#		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
#		Part1	powers/firecontrol/FireBallHitRing1.part
#		Part2	powers/firecontrol/FireBallHitRing2.part
#
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	3
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	chest
#		Part1	Generic\InfernorockBits.part
#		Part2	Generic\InfernorockBitz.part
#	End
#
#
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
