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
		Part1	:GreenPistolflash.part
		Part2	:GreenPistolflashStar.part
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

	