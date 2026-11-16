#########################################################
##	chill touch hit
FxInfo

LifeSpan	250

############################################################

Condition
	On 	Time
	Time 	10

	Event

		ENAME	HandR
		Type	local
		At	WepR

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber_Alpha.part

		Lifespan 80

	End

	Event

		ENAME	HandL
		Type	local
		At	WepL

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber_Alpha.part

		Lifespan 80
	End

End

Condition
	On 	Time
	Time 	20
	Event
		EName	darkskirt
		Type	Start
		At	ROOT

		Part1	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle.part
		Part2	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle2.part
		Sound miasmaburst 100 100 .8
	End

	Event

		ENAME	BaseTrail
		Type	Start
		At	root

		Part1	CustomizeablePowers\DarkMiasma\CreateDarknessTrail.part
		Part2	CustomizeablePowers\DarkMiasma\CreateDarknessTrail2.part


	End

	Event
		EName	darkskirt
		Type	Start
		At	ROOT

		Part1	CustomizeablePowers\DarkMiasma\CreateDarknessPuddleFlat.part
		Part2	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle2Flat.part
	End

	Event

		ENAME	CloudPillar
		Type	Start
		At	Root

		part1	CustomizeablePowers\DarkMiasma\HowlingTwilightMist.part
		part2	CustomizeablePowers\DarkMiasma\HowlingTwilightMist2.part
		Sound miasma_loop 100 100 .5
	End

End
