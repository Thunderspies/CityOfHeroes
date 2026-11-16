#############################################################
## ForceBolts_Hit.fx
#############################################################

FxInfo

LifeSpan 80

#############################################################

Input
	InpName	Chest
End

###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	geom
		Type	local
		At	Chest
		bhvr	Behaviors\SpinHit2.bhvr
		geom	CenterDummy
	End

	Event
		EName 	hit2
		Type	local
		At	Chest
		Part1	POWERS\EnergyBlast\EnergyBallsHit.part
		Part2	POWERS\EnergyBlast\DarkEnergyBallsHit.part
		Part3	POWERS\EnergyBlast\SolidEnergyBallsHit.part
		Part4	POWERS\EnergyBlast\BlastHitRings.part
		Sound PPD_ForceCage_Hit_01 100 100 .75
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