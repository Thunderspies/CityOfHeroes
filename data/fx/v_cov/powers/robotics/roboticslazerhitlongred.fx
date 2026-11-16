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
		
		Part	:EnergyBlastHitGlow1_Red.part
		Part	:EnergyBlastHitGlow1_Red.part
		Part	:EnergyBlastHitGlow1_Red.part
		Part	:EnergyBlastHitStar_Red.part
		Part	:EnergyBlastGlowStreaks_Red.part

	End
	
End

Condition
	On	Cycle
	Time	20
	Chance	.8

	Event
		Type	Start
		At	Chest
		
		Part	:EnergyBlastHitGlow1_Red.part
		Part	:EnergyBlastHitGlow1_Red.part
		Part	:EnergyBlastHitGlow1_Red.part
		Part	:EnergyBlastHitStar_Red.part
		Part	:EnergyBlastGlowStreaks_Red.part

		LifeSpan  5
	End
	
End

End				