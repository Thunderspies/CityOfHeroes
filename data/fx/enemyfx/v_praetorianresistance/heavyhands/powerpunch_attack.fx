#############################################################
## HeavyHands_Activation.fx
#############################################################

FxInfo
Lifespan 35
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepL
	#	Part	ENEMYFX\V_PraetorianResistance\HeavyHands\BluePowerpunch2.part
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunchRings_Burst.part
		Part1	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch_Core_Small_Trail.part
	#	Part	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch_Core_Small.part
		sound HeavyHands_PowerPunch_Attack 100 100 0.8
		Lifespan 25
	End
End

#############################################################

End