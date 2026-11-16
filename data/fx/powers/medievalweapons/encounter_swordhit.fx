#########################################################
##	template

FxInfo


Input
	Inpname Mystic
End

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Mystic
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		Sound SwordStab4 80.0 80.0 1.0
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

Condition
	On Time 
	Time 5

	Event
		EName	all
		Type 	Destroy
			
	End

End

	

End	

	