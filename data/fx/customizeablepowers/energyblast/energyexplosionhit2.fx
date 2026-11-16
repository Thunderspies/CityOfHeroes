#############################################################
## EnergyExplosionHit2.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 80

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
		Type	local
		At	Chest
		Part1	CustomizeablePowers\EnergyBlast\EnergyBallsHit.part
		Part2	CustomizeablePowers\EnergyBlast\DarkEnergyBallsHit.part
		Part3	CustomizeablePowers\EnergyBlast\SolidEnergyBallsHit.part
		Sound explohit 80 80 .75
		Lifespan 25
	End

	Event
		Type	local
		At	Chest
		Part1	CustomizeablePowers\EnergyBlast\EnergyBallsBurst.part
		Part2	CustomizeablePowers\EnergyBlast\EnergyBallsBurst2.part
	End

	Event
		Type	local
		At	Chest
		Part1	CustomizeablePowers\EnergyBlast\EnergyBallsHit.part
		Part2	CustomizeablePowers\EnergyBlast\DarkEnergyBallsHit.part
		Part3	CustomizeablePowers\EnergyBlast\SolidEnergyBallsHit.part
		Part4	CustomizeablePowers\EnergyBlast\BlastHitRings.part
		Part1	CustomizeablePowers\EnergyBlast\EnergyBallsBurst.part
		Part2	CustomizeablePowers\EnergyBlast\EnergyBallsBurst2.part
		Lifespan 7
	End

	Event
		Type	start
		At	geom
		Altpiv	2
		Part1	CustomizeablePowers\EnergyBlast\ExplosionHitRing.part
		Part2	CustomizeablePowers\EnergyBlast\ExplosionHitRing2.part
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