#############################################################
## Consume.fx
#############################################################

FxInfo
LifeSpan		40

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\FieryAura\FireHandz.part
		Sound Firehand_loop 100.0 100.0 .55
		LifeSpan	40

	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\FieryAura\FireHandz.part
		Sound fireball2 100.0 100.0 .8

		LifeSpan	40
	End

End

#############################################################

End
