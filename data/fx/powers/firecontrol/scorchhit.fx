#############################################################
## ScorchHit.fx
#############################################################

FxInfo

Lifespan 70

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	chest
		Part	powers\firecontrol\FireWeaponExplosion.part
		Lifespan 10
	End

	Event
		EName 	flash
		Type	Local
		At	chest
		Part	powers\firecontrol\FireWeaponHitSpark.part
		Lifespan 15
	End

	Event
		EName 	Fire
		Type	local
		At	chest
		Part	:FireBallTargetSteam.part
		Part	:FireBallTargetFlame2.part
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