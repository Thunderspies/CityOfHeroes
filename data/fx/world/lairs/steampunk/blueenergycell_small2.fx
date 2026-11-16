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
		Part1	:BlueEnergyCellSmoke_small2.Part
		#Part2	:BlueEnergyCellElectrons.part
		Part3	:BlueEnergyCellDots.part
		Part4	:BlueEnergyCellRings.part
		
	End

End



End	


			