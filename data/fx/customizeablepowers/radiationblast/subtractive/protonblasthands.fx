#############################################################
## ProtonBlastHands.fx
#############################################################

FxInfo

Lifespan 120

#########################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	Hand1
		Type	Local
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part1	:RadiationInfectOthersHands_Alpha.part

		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .5
	End
End

#############################################################

End