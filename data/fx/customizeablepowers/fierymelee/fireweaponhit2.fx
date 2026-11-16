#############################################################
## FireweaponHit2.fx
#############################################################

FxInfo
LifeSpan	100

########################################################

Input
	Inpname	Hips
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	hips
	#	Part1	CustomizeablePowers\FieryMelee\FireWeaponExplosion.part
	#	Part2	CustomizeablePowers\FieryMelee\FireWeaponHitSpark.part
		Sound atomicblast3 80 80 .7
	End

	Event
		EName 	Fire
		Type	local
		At	hips
		Part1	CustomizeablePowers\FieryMelee\FireBallTargetSteam.part
		Part2	CustomizeablePowers\FieryMelee\FireBallTargetFlame2.part
		Part3	CustomizeablePowers\FieryMelee\FireBallSpark.part
		Sound FireExplo 100.0 100.0 .9
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
		ChildFX CustomizeablePowers\FireBlast\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX CustomizeablePowers\FireBlast\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
End