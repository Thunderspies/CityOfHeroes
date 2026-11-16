#########################################################
##	
##
FxInfo

Input  
	InpName	hips
End

Input  
	InpName	CHEST
End

Input  
	InpName	HEAD
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

##########################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	LLEGL
				
		Part	:GollemStoneArmorRocks.part
		Part	:GollemStoneArmorDirt01.part
		Part	:GollemStoneArmorDirt02.part
		Sound stone_loop 80 80 .9
	End

	Event
		Type	Local 
		At	LLEGR
		
		Part	:GollemStoneArmorRocks.part
		Part	:GollemStoneArmorDirt01.part
		Part	:GollemStoneArmorDirt02.part
	End

###################################################################

	
	Event
		Type	Local 
		At	UARML
		
		Part	:GollemStoneArmorRocks.part
		Part	:GollemStoneArmorDirt01.part
		Part	:GollemStoneArmorDirt02.part
	End

	Event
		Type	Local 
		At	UARMR
		
		Part	:GollemStoneArmorRocks.part
		Part	:GollemStoneArmorDirt01.part
		Part	:GollemStoneArmorDirt02.part
	End

End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		Ename	All
		Type	Destroy
				
	End

End

End	