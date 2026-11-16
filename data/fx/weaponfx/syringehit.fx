#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

LifeSpan 10

Condition
	On Time 
	Time 5
	
	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:Syringeflash.part
		Part2	:SyringeflashStar.part
		part	:SyringeSpark.part
		Sound syringe5 40 40 .65
	End

End

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

End



End	

	