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

LifeSpan	100

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:ShivianHands.part
		Part2	:ShivianHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .4
		LifeSpan	100
	End

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:ShivianHands.part
		Part2	:ShivianHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		LifeSpan	100
	End


End

End	


			