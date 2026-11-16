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
		Part1	:GreenEnergyCellSmoke.Part
		Part2	:GreenEnergyCellElectrons.part
		Part3	:GreenEnergyCellDots.part
		Part4	:GreenEnergyCellRings.part
		
	End

End



End	


			