#########################################################
##	template

FxInfo

LifeSpan	10

Condition
	On	Time	
	Time	0

	Event	
		ENAME	Lightning
		Type	Local
		At	Root
		BHVR	Behaviors/PivotOffsetChest.bhvr
		part	:AOE_Lightning_RING.part
		part	:AOE_Lightning_RINGDebris.part
		
	End

	Event	
		ENAME	Lightning
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
		LifeSpan	10
	End			
	
	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		part	:Storm_Lightning_02Largea.part
		part	:Storm_Lightning_Flash_01.part
		LifeSpan	10
	End			

	Event	
		ENAME	Lightning
		Type	Local
		At	chest
		part	:Storm_Lightning_03Large.part
		LifeSpan	10
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