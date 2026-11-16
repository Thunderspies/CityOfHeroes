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
		#Part4	:IcePatchRing.part
		Sound IcePatch_loop 80 80 .77
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	start
		At	Root
		Part1	:IcePatchSparklies.part
		part2	:IcePatchInsideMist.part
	End
End

Condition
	On	Time
	Time	11

	Event
		Type	start
		At	Root
		part1	:IcePatchMist.part
		part3	:IcePatchMist2.part
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
		Geom	IceChunk_01
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks4.bhvr
		Geom	IceChunk_02
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks3.bhvr
		Geom	IceChunk_03
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks3.bhvr
		Geom	IceChunk_04
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks3.bhvr
		Geom	IceChunk_05
	End

	Event
		EName	geo6
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks.bhvr
		Geom	IceChunk_06
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks2.bhvr
		Geom	IceChunk_07
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks2.bhvr
		Geom	IceChunk_08
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks.bhvr
		Geom	IceChunk_09
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks.bhvr
		Geom	IceChunk_10
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks3.bhvr
		Geom	IceChunk_11
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks.bhvr
		Geom	IceChunk_12
	End

	Event
		Type	start
		At	Root
		bhvr	behaviors\IcePatchChunks2.bhvr
		Geom	IceChunk_13
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		Bhvr	:IcePatchRings.bhvr
		Splat	Splat_ElectricRing.tga
		LifeSpan 8
	End
End

Condition
	On	Time
	Time	17


	Event
		Type	start
		At	Root
		#Part4	:IcePatchRing2.part
		Splat	Splat_ElectricRing.tga
		Bhvr	:IcePatchRings.bhvr
		LifeSpan 1
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	start
		At	Root
		#Part4	:IcePatchRing2.part
		Splat	IceSlick.tga
		Bhvr	:IcePatchSplat.bhvr
	End

#	Event
#		Type	start
#		At	Root
#		bhvr	behaviors\IcePatch.bhvr
#		Geom	Iceslick
#	End
End

Condition
	On	cycle
	Time	25
	Chance	.9

	Event
		Type	start
		At	Root
		Bhvr	:IcePatchRingsSmall.bhvr
		Splat	Splat_ElectricRing.tga
		#Part4	:IcePatchRings.part
		LifeSpan 15
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	start
		At	Root
		Part5	:IcePatchCubes.part
	End
End

#############################################################

End