#############################################################
## FossilHit.fx
#############################################################

FxInfo
ClampMaxScale 5.0 5.0 5.0

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	start
		At	Root
		Part2	:FossilDust2.part
		part3	:FossilDust1.part
		Part	:FossilRocks02.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName 	Stonesickle05
		Type	start
		At	root
		Bhvr	behaviors\FossilcrackScale.bhvr
		geom	Crack01
		Sound explohit 100 100 .95
	End
End

Condition
	On	Time
	Time	10
	Event
		EName	Rocksz
		Type	start
		At	Root
		Bhvr	behaviors\FossilScale.bhvr
		Geom	StoneFossil
	End
End

Condition
	On	Time
	Time	9

	Event
		EName	Rocks2
		Type	start
		At	Root
		Part2	:FossilCrack2.part
		Lifespan 51
	End
End

Condition
	On	Time
	Time	8

	Event
		EName	Rocks3
		Type	start
		At	Root
		Part2	:FossilCrack2.part
		Lifespan 55
	End
End

Condition
	On	Time
	Time	11

	Event
		EName	Rocks4
		Type	start
		At	Root
		Part2	:FossilCrack2.part
		Lifespan 52
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		ChildFX	:Child_RockScatter2.fx
	End

	Event
		HardwareOnly
		Type	Start
		At	Root
		ChildFX	:Child_RockScatter2.fx
	End
End

#############################################################

End