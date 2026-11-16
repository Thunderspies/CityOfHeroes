#########################################################
##	
##

FxInfo

LifeSpan  140

##################################

Condition

	Event
		Type	Start
		At	Chest
		
		Part	:EnergyBlastHitGlow1.part
		Part	:EnergyBlastHitGlow1.part
		Part	:EnergyBlastHitGlow1.part
		Part	:EnergyBlastHitStar.part
		Part	:EnergyBlastGlowStreaks.part

	End
	
End

Condition
	On	Cycle
	Time	20
	Chance	.8

	Event
		Type	Start
		At	Chest
		
		Part	:EnergyBlastHitGlow1.part
		Part	:EnergyBlastHitGlow1.part
		Part	:EnergyBlastHitGlow1.part
		Part	:EnergyBlastHitStar.part
		Part	:EnergyBlastGlowStreaks.part

		LifeSpan  5
	End
	
End

End				