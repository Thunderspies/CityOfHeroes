#############################################################
## IcePatch.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		Sound IcePatch_loop 80 80 0.77
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	start
		At	Root
		Part1	CustomizeablePowers\IceManipulation\IcePatchSparklies.part
		part2	CustomizeablePowers\IceManipulation\IcePatchInsideMist.part
	End
End

Condition
	On	Time
	Time	11

	Event
		Type	start
		At	Root
		part1	CustomizeablePowers\IceManipulation\IcePatchMist.part
		part3	CustomizeablePowers\IceManipulation\IcePatchMist2.part
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_01
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks4.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_02
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_03
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_04
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_05
	End

	Event
		EName	geo6
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_06
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_07
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_08
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_09
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_10
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_11
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_12
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_IceChunk_13
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\IceManipulation\Splat_IcePatch_Rings.bhvr
		Splat	Splat_ElectricRing.tga
		LifeSpan 1
	End
End

Condition
	On	Time
	Time	17

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\IceManipulation\Splat_IcePatch_rings.bhvr
		Splat	Splat_ElectricRing.tga
		LifeSpan 1
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\IceManipulation\Splat_IcePatch.bhvr
		Splat	Tintable_IceSlick.tga
	End
End

Condition
	On	cycle
	Time	25
	Chance	0.9

	Event
		Type	start
		At	Root
		Bhvr	CustomizeablePowers\IceManipulation\Splat_IcePatch_RingsSmall.bhvr
		Splat	Splat_ElectricRing.tga
		LifeSpan 15
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	start
		At	Root
		Part5	CustomizeablePowers\IceManipulation\IcePatchCubes.part
	End
End

#############################################################

End