#############################################################
## BasicPowerPunchHit.fx
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
		Part	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit.part
		Part	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit1.part
		Part	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit2.part
		Part	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHitRaysSmall.part
		Sound PP10 100 100 0.8
	End


	Event
		Type	start
		At	Chest
		Part	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchBallz.part
		Part	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchRingz.part
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