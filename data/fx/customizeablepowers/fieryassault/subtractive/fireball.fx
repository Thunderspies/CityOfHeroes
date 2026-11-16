#############################################################
## FireBall.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part	CustomizeablePowers\FieryAssault\FireHands.part
		Sound Fireball1 100.0 100.0 .8
		LifeSpan 45
	End
End

Condition
	On 	Time
	Time 	10
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part	CustomizeablePowers\FieryAssault\FireHandsFlash.part
		LifeSpan 14
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	CustomizeablePowers\FieryAssault\projectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part	CustomizeablePowers\FieryAssault\FireBallHead.part
		Part	CustomizeablePowers\FieryAssault\FireBallTail.part
		Part	CustomizeablePowers\FieryAssault\FireBallStreak.part
		Part	CustomizeablePowers\FieryAssault\SparkTrail.part

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		120
			PhysForcePowerJitter	25
		End
	End
End

Condition
	On 	primehit
	Repeat  8

	Event
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		ChildFX :FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		EName 	Sparkz
		Type 	Local
		At 	T_Chest
		ChildFX :FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End


Condition
	On	PrimeHit

	Event
		Ename	FireBallHit
		Type	local
		At	target
		Part	CustomizeablePowers\FieryAssault\BigFireExplosion.part
		Part	CustomizeablePowers\FieryAssault\FireBallHitSpark.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	target
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		400
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	T_hips
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

End


