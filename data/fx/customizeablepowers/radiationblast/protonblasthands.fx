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
		Part1	CustomizeablePowers\RadiationBlast\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .5
	End
End

#############################################################

End