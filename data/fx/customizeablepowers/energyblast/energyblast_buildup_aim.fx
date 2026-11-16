#############################################################
## EnergyBlast_BuildUp_Aim.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	4

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyBlast\EnergyBallsHit.part
		Part2	CustomizeablePowers\EnergyBlast\DarkEnergyBallsHit.part
		Part3	CustomizeablePowers\EnergyBlast\SolidEnergyBallsHit.part
		Part4	CustomizeablePowers\EnergyBlast\BlastHitRings.part
		LifeSpan 20
	End
End

## GENERIC FX ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		ChildFX	POWERS\Child_Aim.fx
	End
End

#############################################################

End