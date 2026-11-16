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
		Part1	:EnergyBallsHit.part
		Part2	:DarkEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit_Alpha.part

		Sound explohit 80 80 .75
		Lifespan 25
	End

	Event
		Type	local
		At	Chest
		Part1	:EnergyBallsBurst.part
		Part1	:EnergyBallsBurst_Alpha.part

		Part2	:EnergyBallsBurst2.part
	End

	Event
		Type	local
		At	Chest
		Part1	:EnergyBallsHit.part
		Part2	:DarkEnergyBallsHit.part
		Part3	:SolidEnergyBallsHit.part
		Part4	:BlastHitRings.part
		Part4	:BlastHitRings_Alpha.part

		Part1	:EnergyBallsBurst.part
		Part1	:EnergyBallsBurst_Alpha.part

		Part2	:EnergyBallsBurst2.part
		Lifespan 7
	End

	Event
		Type	start
		At	geom
		Altpiv	2
		Part1	:ExplosionHitRing.part
		Part2	:ExplosionHitRing2.part
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