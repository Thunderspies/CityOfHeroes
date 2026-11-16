#########################################################
##	template

FxInfo

LifeSpan	10

Condition

	Event	
		ENAME	glows
		Type	Local
		At	chest
		

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
		part	:BRIGHTStorm_Heart_01.part
		part	:BRIGHTStorm_Center_01.part
		part	:BRIGHTStorm_Center_02.part

		part	:StormGlows.part

		LifeSpan	1
	End	

	Event	
		ENAME	glows
		Type	Local
		At	chest
		

		part	:StormGlows.part
		LifeSpan	4

	End

End

End		