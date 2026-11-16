#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

Input
	Inpname Emblem
End

Input
	Inpname Head
End


Input
	Inpname Back
End


Input
	Inpname Col_R
End

Input
	Inpname Col_L
End

Input
	Inpname UarmR
End

Input
	Inpname UarmL
End


Input
	Inpname ULegR
End

Input
	Inpname ULegL
End

Input
	Inpname LLegL
End

Input
	Inpname LLegR
End

Input
	Inpname LarmL
End

Input
	Inpname LarmR
End


######## start sparks   #################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Head  ##Chest  ##Emblem  
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		#Sound	lightningbolt4 80 30 .5
	End

End


Condition
	On Time 
	Time 2
	
	Event
		EName	Pow_2
		Type 	Local
		At	UarmR   ##Col_R
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
	End

End


Condition
	On Time 
	Time 4
	
	Event
		EName	Pow_3
		Type 	Local
		At	UarmL
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
	End

End



Condition
	On Time 
	Time 7
	
	Event
		EName	Pow_4
		Type 	Local
		At	UlegR
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
	End

End



Condition
	On Time 
	Time 5
	
	Event
		EName	Pow_5
		Type 	Local
		At	Head
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
	End

End



Condition
	On Time 
	Time 6
	
	Event
		EName	Pow_6
		Type 	Local
		At	LarmL
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
	End

End


#### kill sparks  ##################################

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
	Time 4
	
	Event
		EName	Pow_2
		Type 	Destroy
			
	End

End


Condition
	On Time 
	Time 6
	
	Event
		EName	Pow_3
		Type 	Destroy
			
	End

End


Condition
	On Time 
	Time 10
	
	Event
		EName	Pow_4
		Type 	Destroy
			
	End

End


Condition
	On Time 
	Time 8
	
	Event
		EName	Pow_5
		Type 	Destroy
			
	End

End


Condition
	On Time 
	Time 9
	
	Event
		EName	Pow_6
		Type 	Destroy
			
	End

End




Condition
	On Time 
	Time 30

	Event
		EName	all
		Type 	Destroy
			
	End

End

	

End	

	