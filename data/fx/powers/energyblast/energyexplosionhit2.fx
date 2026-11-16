#############################################################
## EnergyExplosionHit2.fx
#############################################################

FxInfo

LifeSpan 80

#############################################################

Input
	InpName	Chest
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	geom
		Type	local
		At	Root
		geom	fireAura
	End

	Event
		EName 	hit2
		Type	local
		At	Chest
		Part1	POWERS\EnergyBlast\EnergyBallsHit.part
		Part2	POWERS\EnergyBlast\DarkEnergyBallsHit.part
		Part3	POWERS\EnergyBlast\SolidEnergyBallsHit.part
		#Part4	POWERS\EnergyBlast\BlastHitRings.part
		Sound explohit 80 80 0.75
		Lifespan 25
	End

	Event
		EName 	Burst
		Type	local
		At	Chest
		Part1	POWERS\EnergyBlast\EnergyBallsBurst.part
		Part2	POWERS\EnergyBlast\EnergyBallsBurst2.part
	End

	Event
		EName 	Hit
		Type	local
		At	Chest
		Part1	POWERS\EnergyBlast\EnergyBallsHit.part
		Part2	POWERS\EnergyBlast\DarkEnergyBallsHit.part
		Part3	POWERS\EnergyBlast\SolidEnergyBallsHit.part
		Part4	POWERS\EnergyBlast\BlastHitRings.part
		Part1	POWERS\EnergyBlast\EnergyBallsBurst.part
		Part2	POWERS\EnergyBlast\EnergyBallsBurst2.part
		Lifespan 7
	End

	Event
		EName 	Ring
		Type	start
		At	geom
		Altpiv	2
		Part1	POWERS\EnergyBlast\ExplosionHitRing.part
		Part2	POWERS\EnergyBlast\ExplosionHitRing2.part
	End
		Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		600
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

End