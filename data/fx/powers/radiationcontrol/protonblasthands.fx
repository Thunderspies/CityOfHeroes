#########################################################
##	Fire_Ball
##
FxInfo

Input  
	Inpname	WepR
End

#########################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .5
		#LifeSpan	60
	End

	
End

End	


			