#############################################################
## FireBallTarget.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	time
	Time 	0
	Repeat  9

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		hardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
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
		Part1	CustomizeablePowers\FieryAssault\FireBallTargetSteam.part
		Part2	CustomizeablePowers\FieryAssault\FireBallTargetFlame2.part
		Part3	CustomizeablePowers\FieryAssault\FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	CustomizeablePowers\FieryAssault\FireWeaponExplosion.part
		Part2	CustomizeablePowers\FieryAssault\FireWeaponHitSpark.part
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
End

Condition
	On 	Time
	Time 	6
	Event
		EName 	flash
		Type	Destroy

	End


End


End
