#############################################################
## EnergyTransfer_Hit.fx
#############################################################

FxInfo
LifeSpan 45

########################################################

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
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit1.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHit2.part
		Part4	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchHitRaysSmall.part
		Sound PunchHit2 100 100 0.89
	End

	Event
		Type	start
		At	Chest
		Part1	CustomizeablePowers\EnergyMelee\Subtractive\HitRing.part
		Part2	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchBallz.part
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\HitRing2.part
		Part4	CustomizeablePowers\EnergyMelee\Subtractive\PowerPunchRingz.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Explode
		Type	Local
		At	Chest
		Bhvr	CustomizeablePowers\EnergyMelee\Subtractive\CameraShake05Subtle.bhvr
		BhvrOverride
			Shake			0.5
			ShakeFallOff		0.025
			ShakeRadius		32.0
		End
		Part	CustomizeablePowers\EnergyMelee\Subtractive\EnergyTransfer_Hit_ExplosiveFlash.part
		Part	CustomizeablePowers\EnergyMelee\Subtractive\EnergyTransfer_Hit_ExplosiveRings.part
		PMagnet	Chest
		LifeSpan 5
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		1500
			PhysForcePowerJitter	200
		End
		Lifespan 1
	End
End

#############################################################

End
