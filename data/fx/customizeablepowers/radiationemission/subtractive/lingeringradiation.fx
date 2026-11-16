#########################################################
## Radiation - Lingering Radiaiton
#########################################################

FxInfo

LifeSpan 78

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Hand1
		Type	Local
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radinfect1 66 66 .4
		LifeSpan 80
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
		LifeSpan 80
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME	AOE_FX
		Type	Start
		At	T_Root
		ChildFX	:LingeringRadiation_AOE.fx
	End
End

#############################################################

End