#############################################################
## SmallEnergyBlastHit.fx
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
		At	Chest
		bhvr	Behaviors\SpinHit2.bhvr
		geom	CenterDummy
	End

	Event
		EName 	hit2
		Type	local
		At	Chest
		Part1	:EnergyBallsHitRed.part
		Part	:EnergyBallsHitBlack.part
		Part2	:DarkEnergyBallsHitRed.part
		Part	:DarkEnergyBallsHitBlack.part
		Part3	:SolidEnergyBallsHitRed.part
		Part	:SolidEnergyBallsHitBlack.part
		Part4	:BlastHitRingsaRed.part
		Sound wolfpackenergyblasthit2 70 70 0.7
		Lifespan 15
	End

	Event
		EName 	Burst
		Type	local
		At	Chest
		Part2	:EnergyBallsBurst2Red.part
		Part	Powers\ElectricityControl\ElectricityArchContinuingRed.part
		Part	Powers\ElectricityControl\ElectricityArchContinuing2Red.part
	End

	Event
		EName 	Burst
		Type	local
		At	Chest
		Part	Powers\ElectricityControl\ElectricityArch4aRed.part
		Part	Powers\ElectricityControl\ElectricityArchSmallaRed.part
		LifeSpan 20
	End
End

#############################################################

End