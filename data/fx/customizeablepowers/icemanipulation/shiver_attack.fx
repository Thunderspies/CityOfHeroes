#############################################################
## Shiver_Attack.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################
Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	WepL
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
		Sound Coldblast2 100 100 0.8
		LifeSpan 70
	End

	Event
		Type	Local
		At	WepR
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
		LifeSpan 70
	End
End

#############################################################

Condition
	On	Time
	Time	28

	Event
		EName 	Prime
		Type	Local
		At	Head
		Part1	:shiverIce.part
		LifeSpan 55
	End

	Event
		EName 	Prime2
		Type	Local
		At	Head
		Part3	:ShiverSnow.part
		Part3	:ShiverMist.part
		LifeSpan 37
	End
End

#############################################################

End