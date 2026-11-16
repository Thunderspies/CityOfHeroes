#########################################################
##	template

FxInfo


Input
	Inpname Chest

End

LifeSpan 15

Condition
	On Time 
	Time 6
	
	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:EnchantedShotgunflash.part
		Part2	:EnchantedShotgunflashStar.part
		
	End

End

Condition
	On 	Time
	Time	15
		
	Event
		Ename	Prime
		Type	Destroy
	End

End


End	

	