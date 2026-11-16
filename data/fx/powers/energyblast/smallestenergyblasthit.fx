#############################################################
## SmallestEnergyBlastHit.fx
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
		bhvr	Behaviors/SpinHit2.bhvr
		geom	CenterDummy
	End

	Event
		EName 	hit2
		Type	local
		At	Chest
		Part1	:EnergyBallsHit.part
		Part	:EnergyBallsHitWhite.part
		Part3	:SolidEnergyBallsHit2.part
		Part4	:BlastHitRingsa.part
		Sound wolfpackenergyblasthit2 70 70 0.7
		Lifespan 15
	End

	Event
		EName 	Burst
		Type	local
		At	Chest
		Part2	:EnergyBallsBurst2.part
	End
End

#############################################################

End