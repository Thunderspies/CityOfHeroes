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
		Sound cold_loop 100 100 .8
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
		Sound ice1_loop 100 100 .4
		Part1	:IceSlickSparklies.part
		part2	:IceSlickInsideMist.part
	End
End

Condition
	On	Time
	Time	11

	Event
		EName	Pri
		Type	start
		At	Root
		part1	:IceSlickMist.part
		part3	:IceSlickMist2.part
	End
End

Condition
	On	Time
	Time	35

	Event
		EName	Pri
		Type	start
		At	Root
		Part5	:IceSlickCubes.part
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
		Bhvr	:IcePatchRings.bhvr
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
		Bhvr	:IcePatchRings.bhvr
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
		Bhvr	:IceSlickSplat.bhvr
		Splat	IceSlick.tga
	End
End

#############################################################

#Condition
#	On	Time
#	Time	10
#
#	Event
#		EName	splat
#		Type	start
#		At	Root
#		ChildFx	:IceSlickChild.fx
#	End
#End

#############################################################

End