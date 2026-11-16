#########################################################
##	Fire_Ball
##
FxInfo

Input  
	Inpname	Origin
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	Origin
		Part1	:hellionart_CellSmoke.Part
		Part2	:hellionart_CellElectrons.part
		Part3	:hellionart_CellDots.part
		Part4	:hellionart_CellRings.part
		
	End

End



End	


			