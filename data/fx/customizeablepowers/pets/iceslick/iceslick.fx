#############################################################
## Ice Control - Ice Slick
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pri
		Type	start
		At	Root
		Sound cold_loop 100 100 0.8
		LifeSpan 8
	End
End

Condition
	On	Time
	Time	7

	Event
		EName	Pri
		Type	start
		At	Root
		Sound ice1_loop 100 100 0.4
		Part1	CustomizeablePowers\Pets\IceSlick\IceSlickSparklies.part
		part2	CustomizeablePowers\Pets\IceSlick\IceSlickInsideMist.part
	End
End

Condition
	On	Time
	Time	11

	Event
		EName	Pri
		Type	start
		At	Root
		part1	CustomizeablePowers\Pets\IceSlick\IceSlickMist.part
		part3	CustomizeablePowers\Pets\IceSlick\IceSlickMist2.part
	End
End

Condition
	On	Time
	Time	35

	Event
		EName	Pri
		Type	start
		At	Root
		Part5	CustomizeablePowers\Pets\IceSlick\IceSlickCubes.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pri
		Type	start
		At	Root
		Bhvr	POWERS\ColdControl\IcePatchRings.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	80
			SecondaryTint	0
		End
		Splat	Splat_ElectricRing.tga
		LifeSpan	8
	End
End

Condition
	On	Time
	Time	7

	Event
		EName	Pri
		Type	start
		At	Root
		Bhvr	POWERS\ColdControl\IcePatchRings.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	80
			SecondaryTint	0
		End
		Splat	Splat_ElectricRing.tga
		LifeSpan	8
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Pri
		Type	start
		At	Root
		Bhvr	POWERS\ColdControl\IceSlickSplat.bhvr
		BhvrOverride
			Alpha		200
			StartColor	255 255 255
			PrimaryTint	80
			SecondaryTint	0
		End
		Splat	Tintable_IceSlick.tga
	End
End

#############################################################

End