#############################################################
## FireWeaponHit.fx
#############################################################

FxInfo

LifeSpan	100

#############################################################

Input
	InpName	hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	powers/firecontrol/FireWeaponExplosion.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
	End

	Event
		EName 	Fire
		Type	local
		At	hips
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
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
	On 	time
	Time 	0
	Repeat  7
	RepeatJitter	3

	Event
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	Time
	Time 	15
	Event
		EName 	flash
		Type	Destroy

	End


End

Condition
	On 	Time
	Time 	100
	Event
		EName 	Fire
		Type	Destroy

	End

	Event
		EName 	all
		Type	Destroy

	End

End

End