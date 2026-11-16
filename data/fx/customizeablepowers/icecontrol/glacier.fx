#########################################################
##	cot_waterfall_med.fx
##
FxInfo

Input
	InpName	Root
End

Input
	InpName	WepL
End

Input
	InpName	WepR
End

LifeSpan	300

##################################



Condition
	On	Time
	Time	0


	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IceControl\snowburst.part

		LifeSpan		65

	End


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceControl\snowburst.part
		SOund 	coldblast2 100 30 .7

		LifeSpan		65

	End

	Event
		EName	Pri
		Type	start
		At	Root
		Sound cold_loop 100 100 .7
		Part4	CustomizeablePowers\IceControl\IceSlickRing.part

	End

End

Condition
	On	Time
	Time	5


	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IceControl\ColdArmMist.part
		LifeSpan		65
	End


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR

		Part1	CustomizeablePowers\IceControl\ColdArmMist.part
		LifeSpan		65
	End

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL

		Part2	CustomizeablePowers\IceControl\FrostHands.part
		LifeSpan		67

	End


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR

		Part2	CustomizeablePowers\IceControl\FrostHands.part
		LifeSpan		67

	End

End

Condition
	On	Time
	Time	7


	Event
		EName	Pri
		Type	start
		At	Root

		Part1	CustomizeablePowers\IceControl\IceSlickSparklies.part
		#part2	:IceSlickInsideMist.part


	End

	Event
		EName	Pri
		Type	start
		At	Root

		Part4	CustomizeablePowers\IceControl\IceSlickRing2.part


	End

End


Condition
	On	Time
	Time	11


	Event
		EName	Pri
		Type	start
		At	Root

		part1	CustomizeablePowers\IceControl\IceSlickMist.part
		part3	CustomizeablePowers\IceControl\IceSlickMist2.part


	End


End

#######################################################
##IceChunks
#######################################################

Condition
	On	Time
	Time	15


Event
		EName	geo
		Type	start
		At	Root
		#bhvr	behaviors\IceSlickChunks.bhvr
		#Geom	IceChunk_01
		Sound chillingtouch3 100 100 .6

	End

End



Condition
	On	Time
	Time	20


	Event
		EName	Pri
		Type	start
		At	Root
		bhvr	CustomizeablePowers\IceControl\GLACIER.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Fx_Glacier

	End


End

Condition
	On	Time
	Time	10


	Event
		EName	Pri
		Type	start
		At	Root

		Part4	CustomizeablePowers\IceControl\IceSlickRings.part

	End

End

Condition
	On	Time
	Time	35


Event
		EName	Pri
		Type	start
		At	Root

		Part5	CustomizeablePowers\IceControl\IceSlickCubes.part
	End


End



End