#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 1.0 1.0 1.0

#############################################################

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

#############################################################

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
		ENAME	BaseTrail
		Type	posit
		At	root
		Part1	CustomizeablePowers\DarkMiasma\CreateDarknessTrail.part
		Part2	CustomizeablePowers\DarkMiasma\CreateDarknessTrail2.part
		Sound miasma_loop 100 100 .5
	End

	Event
		EName	darkskirt
		Type	posit
		At	ROOT
		Part1	CustomizeablePowers\DarkMiasma\CreateDarknessPuddleFlat.part
		Part2	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle2Flat.part
	End
End

#############################################################

End