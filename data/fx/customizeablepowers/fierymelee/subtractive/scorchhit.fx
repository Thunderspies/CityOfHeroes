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
		Part1	:FireWeaponExplosion.part
		LifeSpan 10
	End

	Event
		EName 	flash
		Type	Local
		At	chest
		Part2	:FireWeaponHitSpark.part
	End

	Event
		EName 	Fire
		Type	local
		At	chest
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
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
		Lifespan 1

	End
End

#############################################################

End



