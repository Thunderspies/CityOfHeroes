#############################################################
## EnergyBlastHit.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	geom
		Type	local
		At	Chest
		Bhvr	CustomizeablePowers\EnergyBlast\SpinHit2.bhvr
		geom	CenterDummy

	End

	Event
		EName 	hit2
		Type	local
		At	Chest
		Part1	CustomizeablePowers\EnergyBlast\EnergyBallsHit.part
		Part2	CustomizeablePowers\EnergyBlast\DarkEnergyBallsHit.part
		Part3	CustomizeablePowers\EnergyBlast\SolidEnergyBallsHit.part
		Part4	CustomizeablePowers\EnergyBlast\BlastHitRings.part
		Sound explohit2 200 200 .76
		Lifespan 25
	End


	Event
		EName 	Burst
		Type	local
		At	Chest
		Part1	CustomizeablePowers\EnergyBlast\EnergyBallsBurst.part
		Part2	CustomizeablePowers\EnergyBlast\EnergyBallsBurst2.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		1000
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

End