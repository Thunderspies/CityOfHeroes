#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300


#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand1
		Type	Local
		At	UarmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkMiasma\DarkHandsA.part
		Part2	CustomizeablePowers\DarkMiasma\DarkHandsB.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb_Alpha.part

		Sound miasma3 100 100 .8

		PMagnet	LarmR
		Lifespan 93
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkMiasma\DarkHandsA.part
		Part2	CustomizeablePowers\DarkMiasma\DarkHandsB.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb_Alpha.part

		PMagnet	LarmL
		Lifespan 93

	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local
		At	LarmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkMiasma\DarkHands.part
		Part2	CustomizeablePowers\DarkMiasma\DarkHands2.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb_Alpha.part

		PMagnet	WepR
		Lifespan 78
	End



	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkMiasma\DarkHands.part
		Part2	CustomizeablePowers\DarkMiasma\DarkHands2.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmberb_Alpha.part

		PMagnet	WepL
		Lifespan 78

	End

End

#########################################################


End


