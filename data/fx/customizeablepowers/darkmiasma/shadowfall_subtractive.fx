#########################################################
##	chill touch hit
FxInfo



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
		Lifespan 90

	End

	Event

		ENAME	HandL
		Type	local
		At	WepL

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Lifespan 90
	End

End

Condition
	On 	Time
	Time 	0

	Event

		ENAME	Base
		Type	posit
		At	root

		Bhvr	CustomizeablePowers\DarkMiasma\DarkScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave
		Sound miasmaburst 100 100 .6

	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName	darkskirt
		Type	Posit #Start
		At	root

		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle_Subtractive_Local.part
		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle_Subtractive_LocalTrail.part
		Part1	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle_Subtractive.part
		Part2	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle2_Subtractive.part

	End


	Event

		ENAME	BaseTrail
		Type	Posit
		At	root

		Part1	CustomizeablePowers\DarkMiasma\CreateDarknessTrail_Subtractive.part
		Part2	CustomizeablePowers\DarkMiasma\CreateDarknessTrail2_Subtractive.part


	End

	Event
		EName	darkskirt
		Type	Posit #Start
		At	root

		Part1	CustomizeablePowers\DarkMiasma\CreateDarknessPuddleFlat_Subtractive.part
		Part2	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle2Flat_Subtractive.part
	End
End

End