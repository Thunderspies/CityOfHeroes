#############################################################
## FireBallTarget.fx
#############################################################

FxInfo
LifeSpan 200

#############################################################

Input
	Inpname	Hips
End

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
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	powers/firecontrol/FireWeaponExplosion.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
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

Condition
	On 	Time
	Time 	100

	Event
		EName 	All
		Type	Destroy
	End


End


End
