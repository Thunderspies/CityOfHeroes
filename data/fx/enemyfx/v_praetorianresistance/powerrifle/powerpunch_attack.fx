#############################################################
## HeavyHands_Activation.fx
#############################################################

FxInfo

LifeSpan 45

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepL
		Part1	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunch1.part
		Part2	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunch2.part
		Part1	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunch3.part
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunchGlow.part
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunchRings.part
		sound HeavyHands_PowerPunch_Attack 100 100 0.8
	End
End

#############################################################

End