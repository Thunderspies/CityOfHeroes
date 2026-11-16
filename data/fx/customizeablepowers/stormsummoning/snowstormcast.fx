#########################################################
##	RadiationAura
FxInfo
Lifespan 80

##################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound coldblast2 100 100 .9
	End

End

Condition
	On	Time
	Time	10

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL

		Part2	CustomizeablePowers\StormSummoning\ColdArmMist.part
		Part1	CustomizeablePowers\StormSummoning\snowburst.part

		LifeSpan	70
	End


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR

		Part2	CustomizeablePowers\StormSummoning\ColdArmMist.part
		Part1	CustomizeablePowers\StormSummoning\snowburst.part

		LifeSpan	70

	End
End

Condition
	On	Time
	Time	20

	Event
		Type	local
		At	Origin
		Sound cold_loop 100 100 .8
	End

End

