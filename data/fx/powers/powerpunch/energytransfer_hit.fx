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
		Part1	:PowerPunchHit.part
		Part2	:PowerPunchHit1.part
		Part3	:PowerPunchHit2.part
		Part4	:PowerPunchHitRaysSmall.part
		Sound PunchHit2 100 100 .89
	End

	Event
		Type	start
		At	Chest
		Part1	:HitRing.part
		Part2	:PowerPunchBallz.part
		Part3	:HitRing2.part
		Part4	:PowerPunchRingz.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Explode
		Type	Local
		At	Chest
		Bhvr	behaviors\CameraShake05Subtle.bhvr
		BhvrOverride
			Shake			0.5
			ShakeFallOff		0.025
			ShakeRadius		32.0
		End
		Part	:EnergyTransfer_Hit_ExplosiveFlash.part
		Part	:EnergyTransfer_Hit_ExplosiveRings.part
		Part	:EnergyTransfer_Hit_ExplosiveRays.part
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
