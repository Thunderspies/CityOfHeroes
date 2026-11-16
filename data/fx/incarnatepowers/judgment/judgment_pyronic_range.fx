#############################################################
## XXLargeFireBall.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	WepR
#		Part1	:FireHands.part
#		Lifespan 45
#	End
#End
#
#Condition
#	On 	Time
#	Time 	5
#
#	Event
#		Type 	Local
#		At	Origin
#		Sound Fireball1 100.0 100.0 .8
#	End
#End
#
#Condition
#	On 	Time
#	Time 	10
#
#	Event
#		Type	Local
#		At	WepR
#		Part1	:FireHandsFlash.part
#		Sound Fireballfly3 80 80 .9
#		Lifespan 14
#	End
#End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	20
		End
		Part1   :judgement_pyronic_projectilemiddle.part
		Part2   :judgement_pyronic_projectile.part
#		Part1	:XXLargeFireBallHead.part
#		Part2	:FireBallTail.part
#		Part3	:XXLargeFireBallStreak.part
#		Part4	:SparkTrail.part
#		Part2	:Blaze_smoke.part
#		Part3	:Blaze_smoke2.part
#		Part1	:XXLargeFireBallHeadCore.part
		Magnet	Target
		LookAt	Target
		Lifespan 30
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	ExplosiveForce
		Type	Start
		At	target
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End

	Event
		Ename	Prime
		Type	Destroy
	End
End


Condition
	On	PrimeHit

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	target
		Part1	:Judgment_Pyronic_AreaFire_Range.part
		Part1	:Judgment_Pyronic_AreaFireB_Range.part


		Lifespan 3
#		Part2	:FireBallHitSpark.part
	End
End
Condition
	On	Time
	Time	45

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	target

		Part1	:Judgment_Pyronic_smoke_Range.part

		Lifespan 10

#		Part2	:FireBallHitSpark.part
	End
End

Condition
	On	PrimeHit

	Event
		Ename	FireBallHit
		Type	StartPositOnly
		At	target
		BhvrOverride
          		PositionOffset 0 -4 0

         	End
		Part1	:Judgment_Pyronic_FloorFlame_Range.part
		Part1	:Judgment_Pyronic_AreaBurnedEarth_Range.part
	Lifespan 1

	End
End
#
#	Event
#		EName 	Ringnode
#		Type	Local
#		At	T_Root
#		Geom	RootToChestAdjustment
#		Sound Atomicblast 100 100 .9
#	End
#
#	Event
#		EName 	Rings
#		Type	start
#		At	Ringnode
#		altpiv	1
#		bhvr    behaviors\powers\firecontrol\Infernobubble.bhvr
#		Part1	:FireBallHitRing1.part
#		Part2	:FireBallHitRing2.part
#	End
#
#	Event
#		Type	Local
#		At	T_Chest
#		Part1	:InfernorockBits.part
#		Part2	:InfernorockBitz.part
#		Sound FireExplo 100.0 100.0 .6
#		Lifespan 1
#	End
#
###	Event
###		EName 	Sparkz
###		Type 	Local
###		At 	T_hips
###		ChildFX :FireBurstComet.fx
###		Lifespan 70
###		LifespanJitter 20
###	End
#End
#
#Condition
#	On 	PrimeHit
#	Repeat  8
#
#	Event
#		EName 	Sparkz
#		Type 	Local
#		At 	T_hips
#		ChildFX :FireBurstExplode.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
#
#	Event
#		EName 	Sparkz
#		Type 	Local
#		At 	T_hips
#		ChildFX :FireBurstUP.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
#
#	Event
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	T_Chest
#		ChildFX :FireBurstExplode.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
#End

#############################################################

End