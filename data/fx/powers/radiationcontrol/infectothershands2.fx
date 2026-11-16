#########################################################
##	Fire_Ball
##
FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

LifeSpan	50
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound mutationthrow 66 66 .75
		LifeSpan	50
	End

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
	
		LifeSpan	50
	End
	
End

End	


			