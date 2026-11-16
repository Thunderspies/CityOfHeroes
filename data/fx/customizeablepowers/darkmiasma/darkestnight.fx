#############################################################
## DarkestNight.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	WepR
End

Input
	InpName	WepL
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		ENAME	HandR
		Type	local
		At	WepR
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		Sound miasmaburst 100 100 0.6
		Lifespan 85
	End

	Event
		ENAME	HandL
		Type	local
		At	WepL
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		Lifespan 85
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	OrbMist
		Type	Posit
		At	root
		bhvr	behaviors\Rune_spinFast.bhvr
		geom	FX_RuneStreak
	End

	Event
		EName 	OrbStreak
		Type	Posit
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors\Rune_spinFaster.bhvr
	End

	Event
		EName 	Orb
		Type	Local
		At	OrbStreak
		altpiv  1
		part1	:DarkessChillMistTrail.part
		part2	:DarkessChillMistTrail2.part
		Sound miasma_loop 100 100 0.5
	End

	Event
		EName	darkskirt
		Type	posit
		At	ROOT
		Part1	:CreateDarknessPuddle.part
		Part2	:CreateDarknessPuddle2.part
	End

	Event

		ENAME	BaseTrail
		Type	posit
		At	root
		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
	End

	Event
		EName	darkskirt
		Type	posit
		At	ROOT
		Part1	:CreateDarknessPuddleFlat.part
		Part2	:CreateDarknessPuddle2Flat.part
	End
End

Condition
	On 	Time
	Time 	135

	Event
		EName 	OrbStreak2
		Type	Posit
		At	root
		bhvr	behaviors\Rune_spinFaster.bhvr
		geom	FX_RuneStreak
	End

	Event
		EName 	Orb2
		Type	Local
		At	OrbStreak2
		altpiv  1
		part1	:DarkessChillMistTrail.part
		part2	:DarkessChillMistTrail2.part
	End
End

#############################################################

End