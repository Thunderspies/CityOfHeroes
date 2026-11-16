#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

Flags    InheritAlpha

Lifespan 30

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		Sound FRKHit1 70.0 70.0 .8
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

	