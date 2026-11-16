#########################################################
##	template

FxInfo

LifeSpan	10

Condition
	
	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		
		Part	:EnergyRing.part
		part	:StormGlows.part
		part	:StormGlows.part		
		part	:StormGlows.part
		part	:StormGlows.part
		part	:StormGlows.part
		part	:StormGlows.part
		part	:StormGlows.part
		part	:StormGlows.part
		part	:StormGlows.part
		part	:StormGlows.part
	End

	
End

Condition
	On	Cycle
	Time	1

	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		part	:Storm_Lightning_01.part
		part	:Storm_Lightning_Flash_01.part
		part	:WhiteFlash.part
		LifeSpan	4
	End			
	
	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		part	:Storm_Lightning_02.part
		part	:Storm_Lightning_Flash_01.part
		LifeSpan	4
	End			

	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		part	:Storm_Lightning_03.part
		LifeSpan	4
		
	End			

	Event	
		ENAME	Cloud
		Type	Local
		At	chest
		part	:Storm_Heart_01.part
		part	:Storm_Center_01.part
		part	:Storm_Center_02.part
		LifeSpan	1
	End	
End


End		