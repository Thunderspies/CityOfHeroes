#########################################################
##	chill touch hit
FxInfo
LifeSpan	250

############################################################

Condition
	On 	Time
	Time 	10

	Event

		ENAME	hookupW/Spin
		Type	posit
		At	root
		BHVR	CustomizeablePowers\DarkMiasma\SplatRotate.bhvr
	End

	Event
		ENAME	HandR
		Type	local
		At	WepR

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
	End

	Event
		ENAME	HandL
		Type	local
		At	WepL

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
	End

End

Condition
	On	cycle
	Time	15

	Event

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\DarkMiasma\DarktendrilInDark2.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30
	End

	Event

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\DarkMiasma\DarktendrilA.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30

	End

	Event

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\DarkMiasma\DarktendrilInA.bhvr

		Splat	DarknessSplat2.tga

		LifeSpan	30
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName	darkskirt
		Type	Posit #Start
		At	root

		Part	CustomizeablePowers\DarkMiasma\SubtractiveFlash1.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle_Subtractive_Local.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle2_Subtractive.part
	End

	Event

		ENAME	BaseTrail
		Type	Posit
		At	root

		Part	CustomizeablePowers\DarkMiasma\CreateDarknessTrail_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessTrail2_Subtractive.part


	End

	Event
		EName	darkskirt
		Type	Posit #Start
		At	root

		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddleFlat_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle2Flat_Subtractive.part
	End

	Event
		EName	darkskirt
		Type	Start
		At	ROOT

		Part1	:CreateDarknessPuddle.part
		Part2	:CreateDarknessPuddle2.part
		Sound miasmaburst 100 100 .8
	End

	Event

		ENAME	BaseTrail
		Type	Start
		At	root

		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part


	End

	Event
		EName	darkskirt
		Type	Start
		At	ROOT

		Part1	:CreateDarknessPuddleFlat.part
		Part2	:CreateDarknessPuddle2Flat.part
	End

	Event

		ENAME	CloudPillar
		Type	Start
		At	Root

		part1	:HowlingTwilightMist.part
		part2	:HowlingTwilightMist2.part


		part1	CustomizeablePowers\DarkMiasma\HowlingTwilightMist_Subtractive.part
		part2	CustomizeablePowers\DarkMiasma\HowlingTwilightMist2_Subtractive.part


			Sound miasma_loop 100 100 .5
	End

End

Condition
	On 	Time
	Time	90
	Event
		EName 	HandL
		Type	Destroy

	End

	Event
		EName 	HandR
		Type	Destroy

	End
End

End