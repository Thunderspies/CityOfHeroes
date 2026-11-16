#############################################################
## HeavyHands_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepL
#		Part1	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunch1.part
		Part	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunch2.part
		Part	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerPunch_Rings_Small.part
		Part	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch_Core_Small.part
#		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunchGlow.part
		Sound BluePowerup7 100 100 0.7
	End

	Event
		EName 	Prime
		Type	Local
		At	WepR
#		Part1	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunch1.part
		Part	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunch2.part
		Part	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerPunch_Rings_Small.part
		Part	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch_Core_Small.part
#		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunchGlow.part
		Sound HeavyHands_Loop 50 50 0.05
End

#############################################################

End