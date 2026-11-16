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
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		Lifespan 80
	End

	Event

		ENAME	HandL
		Type	local
		At	WepL

		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
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
		Bhvr 	Behaviors/DarkScale3.bhvr
		Geom	FX_DarkWave
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
		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
		Sound miasma_loop 100 100 .5
	End

	Event
		EName	darkskirt
		Type	posit
		At	ROOT
		Part1	:CreateDarknessPuddleFlat.part
		Part2	:CreateDarknessPuddle2Flat.part
	End
End

#############################################################

End