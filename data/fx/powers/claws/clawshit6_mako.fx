#########################################################
##	template

FxInfo


Lifespan	90

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part1	:Clawflash.part
#		Part3	:ClawEmbers.part
#		Part4	:ClawEmbersBroad.part
		Part5	:ClawsStar.part
		Sound spinhit 100.0 100.0 .7
	End

End

Condition
	On Time 
	Time 2
	
	Event
		EName	Pow
		Type 	Destroy
			
	End

End


	

End	

		

	