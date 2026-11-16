#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	Head
		BhvrOverride
			PositionOffset 0 -.3 .5
		End
		Part1	\POWERS\FireControl\FireHands.part
		
		Lifespan	60
		
	
	End


End

Condition
	On 	Time
	Time 	5	

	Event
		Type 	Local
		At	Origin	
		Sound Fireball1 100.0 100.0 .8
	End

End


Condition
	On 	Time
	Time 	10
	Event
		EName	SwordFlash2
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset 0 -.3 .5
		End
		Part1	\POWERS\FireControl\FireHandsFlash.part
		Sound Fireballfly3 80 80 .9
		Lifespan	30
	
	End

End


Condition
	On 	Time
	Time 	36

	Event
		EName 	Prime
		Type	Start 
		At	Head
		Bhvr	behaviors/projectile1.bhvr
		BhvrOverride
			PositionOffset 0 -1.6 .3

			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	20
		End
		Magnet	Target
		LookAt	Target
		Part1	powers/firecontrol/XXLargeFireBallHead.part
		Part2	powers/firecontrol/FireBallTail.part
		Part3	powers/firecontrol/XXLargeFireBallStreak.part
		Part4	\POWERS\FireControl\SparkTrail.part
		Part5	powers/firecontrol/FireBallTail.part
	
		Part1	powers/firecontrol/FireBallTail.part
		Part2	\POWERS\FireControl\Blaze_smoke.part
		Part3	\POWERS\FireControl\Blaze_smoke2.part
		Part1	powers/firecontrol/XXLargeFireBallHeadCore.part
	End
End

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

		Lifespan	1

	End
	
	Event
		Ename	FireBallHit
		Type	local
		At	target
		Part1	\POWERS\FireControl\BigFireExplosion.part
		Part2	\POWERS\FireControl\FireBallHitSpark.part
	End

	Event
		EName 	Ringnode
		Type	Local 
		At	T_Root
		Geom	RootToChestAdjustment
		Sound Atomicblast 100 100 .9

	End

	Event
		EName 	Rings
		Type	start 
		At	Ringnode
		altpiv	1
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers/firecontrol/FireBallHitRing1.part
		Part2	powers/firecontrol/FireBallHitRing2.part
		
	End

#	Event 
#		EName 	PhysicsRing
#		Type 	start
#		At 	Rings
#
#		ChildFX \POWERS\FireControl\PhysicsRing.fx
#	End
#
#	Event 
#		EName 	PhysicsRing2
#		Type 	start
#		At 	Rings
#
#		ChildFX \POWERS\FireControl\PhysicsRing2.fx
#	End
#
	Event
		EName 	Rocks
		Type	Local 
		At	T_Chest
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		Sound FireExplo 100.0 100.0 .6
	End

	Event
		Ename	Rocks
		Type	Destroy
	End

	Event
		Ename	Prime
		Type	Destroy
	End


End

Condition
	On 	primehit
	Repeat  8

	Event 
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	primehit
	Repeat  8

	Event 
		EName 	Sparkz
		Type 	Local
		At 	T_Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	primehit
	Repeat  8

	Event 
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	primehit
	Repeat  2

	Event 
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		ChildFX V_COV\PhysicsEnabled\FireBurstComet.fx
		Lifespan 70
		LifespanJitter 	20
	End
End
End	


			
