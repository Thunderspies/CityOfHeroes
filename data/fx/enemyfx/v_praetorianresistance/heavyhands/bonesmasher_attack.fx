#############################################################
## BoneSmasher_Attack.fx
#############################################################

FxInfo

Lifespan 55

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunchSmoke_Stationary.part
		Lifespan	17
	End

	Event
		Type	Local
		At	WepL
		Part2	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch2.part
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunchGlow.part
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunchRings.part
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunchSmoke.part

		sound HeavyHands_BoneSmasher_Attack 100 100 0.8
		Lifespan 20
		End

	Event
		Type	Local
		At	WepL
		Part1	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch_Core.part
		Part1	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch3.part

	End

	Event
		Type	Local
		At	WepR
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunchSmoke_Stationary.part
		Lifespan	17
	End

	Event
		Type	Local
		At	WepR

		Part2	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch2.part
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunchGlow.part
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunchRings.part
		Part3   ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunchSmoke.part
		Lifespan	20
	End

	Event
		Type	Local
		At	WepR
		Part1	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch_Core.part
		Part1	ENEMYFX\V_PraetorianResistance\HeavyHands\BlueLargepowerpunch3.part

	End
End

#############################################################

End