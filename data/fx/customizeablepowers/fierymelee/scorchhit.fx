#############################################################
## ScorchHit.fx
#############################################################

FxInfo
Lifespan 70

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	chest
		Part1	CustomizeablePowers\FieryMelee\FireWeaponExplosion.part
		LifeSpan 10
	End

	Event
		EName 	flash
		Type	Local
		At	chest
		Part2	CustomizeablePowers\FieryMelee\FireWeaponHitSpark.part
	End

	Event
		EName 	Fire
		Type	local
		At	chest
		Part1	CustomizeablePowers\FieryMelee\FireBallTargetSteam.part
		Part2	CustomizeablePowers\FieryMelee\FireBallTargetFlame2.part
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
		Sound FireExplo 100.0 100.0 0.8
		Lifespan 1

	End
End

#############################################################

End



