#############################################################
## PowerPunchQuickHit.fx
#############################################################

FxInfo
Lifespan 100

## FALLBACK NODES ###########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start
		At	Chest
		Part1	CustomizeablePowers\EnergyMelee\PowerPunchHit.part
		Part2	CustomizeablePowers\EnergyMelee\PowerPunchHit1.part
		Part4	CustomizeablePowers\EnergyMelee\PowerPunchHitRaysSmall.part
		Sound PP10 100 100 0.8
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Mallet
		Type	start
		At	Chest
		Part1	CustomizeablePowers\EnergyMelee\PowerPunchHit.part
		Part2	CustomizeablePowers\EnergyMelee\PowerPunchHit1.part
		Part3	CustomizeablePowers\EnergyMelee\PowerPunchHit2.part
		Part4	CustomizeablePowers\EnergyMelee\PowerPunchHitRaysSmall.part
		Sound PP10 100 100 0.8
	End

	Event
		Type	start
		At	Chest
		Part2	CustomizeablePowers\EnergyMelee\PowerPunchBallz.part
		Part4	CustomizeablePowers\EnergyMelee\PowerPunchRingz.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End
End

#############################################################

End