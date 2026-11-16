#########################################################
##	Fire_Ball
##
FxInfo


LifeSpan	200

Condition
	On 	Time
	Time 	10

	Event
		EName	InitialSwordFire
		Type	Local 
		At	Head
		BhvrOverride
			Positionoffset	0 -.45 .6
		End
		Part1	:FireHands.part
		Lifespan	45
		
	
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
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	Head
		Part1	:FireHandsFlash.part
		BhvrOverride
			Positionoffset	0 -.4 .5
		End
		Sound Fireballfly3 80 80 .9
		Lifespan	29
	
	End

End




Condition
	On 	Time
	Time 	28

	Event
		EName 	Prime
		Type	Start 
		At	Head
		Bhvr	behaviors/projectile1.bhvr
		BhvrOverride
			Positionoffset	0 -4 2
			
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		350
			PhysForcePowerJitter	50
		End
		Magnet	Target
		LookAt	Target
		Part1	:XXLargeFireBallHead.part
		Part2	:FireBallTail.part
		Part3	:XXLargeFireBallStreak.part
		Part4	:SparkTrail.part
		Part5	:FireBallTail.part
	
		Part1	:FireBallTail.part
		Part2	:Blaze_smoke.part
		Part3	:Blaze_smoke2.part
		Part1	:XXLargeFireBallHeadCore.part
		
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
			PhysForceRadius		45
			PhysForcePower		1200
			PhysForcePowerJitter	200
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
	Repeat  24

	Event 
		EName 	Sparkz
		Type 	Local
		At 	Ringnode
		altpiv	1
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	primehit
	Repeat  11

	Event 
		EName 	Sparkz
		Type 	Local
		At 	Ringnode
		altpiv	1
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	primehit
	Repeat  15

	Event 
		EName 	Sparkz
		Type 	Local
		At 	Ringnode
		altpiv	1
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	primehit
	Repeat  4

	Event 
		EName 	Sparkz
		Type 	Local
		At 	Ringnode
		altpiv	1
		ChildFX V_COV\PhysicsEnabled\FireBurstComet.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

End	


			
