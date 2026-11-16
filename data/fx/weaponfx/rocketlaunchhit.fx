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
		Part1	:Shotgunflash.part
		Part2	:ShotgunflashStar.part
		#Sound	explohit2 100.0 60.0 1.0
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

	