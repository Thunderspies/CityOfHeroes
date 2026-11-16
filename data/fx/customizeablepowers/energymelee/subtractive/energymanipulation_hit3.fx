#############################################################
## PowerPunchHit3.fx
#############################################################

FxInfo

LifeSpan 100

## FALLBACK NODES ###########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Chest
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit1.part
		Part4	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHitRaysSmall.part
		Sound PP13 100 100 0.85
	End

	Event
		Type	start
		At	Chest
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\HitRing.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchBallz.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\HitRing2.part
		Part4	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchRingz.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		350
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End
End

#############################################################

End
