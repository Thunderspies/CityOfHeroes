#########################################################
## Radiation Poisoning - Radiation Infection - Activation
#########################################################

FxInfo

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

Condition
	On	Time
	Time	0

	Event

		Type	Local
		At	WepL
		Sound hotignite 70 70 .44
		End
End

Condition
	On 	Time
	Time 	15


	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		Sound flameburst_loop 66 66 .4
		LifeSpan	90
		
	End
	
End

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

End