#############################################################
## StealPower.fx
#############################################################

FxInfo
LifeSpan	70

##################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	x
		Type	start
		At	Root
		part1	CustomizeablePowers\DarkMelee\TendrilRing.part
		part2	CustomizeablePowers\DarkMelee\TendrilRingFlat.part
		Sound Darkmoonbeam3 80 80 .8

		LifeSpan	46
	End

End

Condition
	On	Time
	Time	43

	Event
		EName	x
		Type	start
		At	Root
		part1	CustomizeablePowers\DarkMelee\TendrilRingFlatBurst.part
		part2	CustomizeablePowers\DarkMelee\TendrilRingFlatBurst2.part
		LifeSpan	3
	End

End

Condition
	On	Time
	Time	45

	Event
		EName	y
		Type	start
		At	Root

		Splat	Generic_Ring.tga
		Bhvr	CustomizeablePowers\DarkMelee\BlackYellowRingIn.bhvr

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	49

	Event
		EName	y
		Type	start
		At	Root

		Splat	ThinRing.tga
		Bhvr	CustomizeablePowers\DarkMelee\BlackYellowRingIn2.bhvr

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	53

	Event
		EName	y
		Type	start
		At	Root

		Splat	Generic_Ring.tga
		Bhvr	CustomizeablePowers\DarkMelee\BlackYellowRingIn3.bhvr

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	55

	Event
		EName	y
		Type	start
		At	Root

		Splat	ThinRing.tga
		Bhvr	CustomizeablePowers\DarkMelee\BlackYellowRingIn4.bhvr

		LifeSpan	10
	End
End

Condition

	Event
		EName	15
		Type	start
		At	Chest
		part1	CustomizeablePowers\DarkMelee\HighLightRays.part
		part2	CustomizeablePowers\DarkMelee\HighLightStreaks.part
		part3	CustomizeablePowers\DarkMelee\HighLightRaysMulticolor.part
		part4	CustomizeablePowers\DarkMelee\HighLightStreaksMulticolor.part
	End

End


End