#########################################################
##	FireRing
##

FxInfo

LifeSpan	100

Condition
	On 	Time
	Time 	10

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHandz.part
		Part1	:FireHandz_Alpha.part
		Sound Firehand_loop 100.0 100.0 .55
		LifeSpan	60

	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Part1	:FireHandz_Alpha.part
		Sound Ignite2 100.0 100.0 .9
		LifeSpan	60
	End

End


End