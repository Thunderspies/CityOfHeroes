#############################################################
## WolfPackEnergyBlastHit.fx
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
		bhvr	Behaviors/SpinHit2.bhvr
		geom	CenterDummy
		Sound vandalblast2 80 80 0.8
	End

	Event
		EName 	hit2
		Type	local
		At	Chest
		Part	POWERS\EnergyBlast\EnergyBallsHitWhite.part
		Part1	POWERS\EnergyBlast\EnergyBallsHitRed.part
		Part	POWERS\EnergyBlast\EnergyBallsHitBlack.part
		Part2	POWERS\EnergyBlast\DarkEnergyBallsHitRed.part
		Part	POWERS\EnergyBlast\DarkEnergyBallsHitBlack.part
		Part3	POWERS\EnergyBlast\SolidEnergyBallsHitRed.part
		Part	POWERS\EnergyBlast\SolidEnergyBallsHitBlack.part
		Part4	POWERS\EnergyBlast\BlastHitRingsARed.part
		Sound wolfpackenergyblasthit 80 80 0.76
		LifeSpan 25
	End


	Event
		EName 	Burst
		Type	local
		At	Chest
		Part2	POWERS\EnergyBlast\EnergyBallsBurst2Red.part
		Part	Powers/ElectricityControl/ElectricityArchContinuingRed.part
		Part	Powers/ElectricityControl/ElectricityArchContinuing2Red.part
	End

	Event
		EName 	Burst
		Type	local
		At	Chest
		Part	Powers/ElectricityControl/ElectricityArch4aRed.part
		Part	Powers/ElectricityControl/ElectricityArchSmallaRed.part
		LifeSpan 31.5
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		750
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

End