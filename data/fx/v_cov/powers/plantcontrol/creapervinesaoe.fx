#############################################################
## CreaperVinesAOE.fx
#############################################################

FxInfo

Lifespan 3600

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Origin
		Sound tree_emerge 70 70 0.7
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Origin
		Sound tar_loop 70 70 0.5
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	ArrowHit1
		Type	start
		At	t_root
		Geom	HotFeet
		Bhvr	V_COV\Powers\PlantControl\RootPositionFade.bhvr
		Sound summon2 80.0 80.0 0.7
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Offset1
		Type	start
		At	T_Root
		Bhvr	V_COV\Powers\PlantControl\RootPositionFade.bhvr
		Geom	HotFeet
		Sound summon2 90.0 90.0 0.7
		Lifespan 400
	End

	Event
		EName	Offset2
		Type	start
		At	T_Root
		Bhvr	V_COV\Powers\PlantControl\RootPositionFade2.bhvr
		Geom	HotFeet01
		Lifespan 400
	End
End

#############################################################

Condition
	On 	time
	Time 	2

	Event
		Type	Local
		At	Offset1
		altpiv	1
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	3
		BHVR	V_COV\Powers\PlantControl\RootScale2.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	Offset1
		altpiv	2
		BHVR	V_COV\Powers\PlantControl\RootScale3a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	4
		BHVR	V_COV\Powers\PlantControl\RootScale4a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	5

	Event
		Type	Local
		At	Offset1
		altpiv	3
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	5
		BHVR	V_COV\Powers\PlantControl\RootScale2.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	7

	Event
		Type	Local
		At	Offset1
		altpiv	4
		BHVR	V_COV\Powers\PlantControl\RootScale3a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	6
		BHVR	V_COV\Powers\PlantControl\RootScale4a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	9

	Event
		Type	Local
		At	Offset1
		altpiv	5
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	7
		BHVR	V_COV\Powers\PlantControl\RootScale2.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	11

	Event
		Type	Local
		At	Offset1
		altpiv	6
		BHVR	V_COV\Powers\PlantControl\RootScale3a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	8
		BHVR	V_COV\Powers\PlantControl\RootScale4a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	13

	Event
		Type	Local
		At	Offset1
		altpiv	7
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	9
		BHVR	V_COV\Powers\PlantControl\RootScale2.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	15

	Event
		Type	Local
		At	Offset1
		altpiv	8
		BHVR	V_COV\Powers\PlantControl\RootScale3a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	10
		BHVR	V_COV\Powers\PlantControl\RootScale4a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	17

	Event
		Type	Local
		At	Offset1
		altpiv	9
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	11
		BHVR	V_COV\Powers\PlantControl\RootScale2.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	19

	Event
		Type	Local
		At	Offset1
		altpiv	10
		BHVR	V_COV\Powers\PlantControl\RootScale3a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	12
		BHVR	V_COV\Powers\PlantControl\RootScale4a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	21

	Event
		Type	Local
		At	Offset1
		altpiv	11
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	13
		BHVR	V_COV\Powers\PlantControl\RootScale2.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	23

	Event
		Type	Local
		At	Offset1
		altpiv	12
		BHVR	V_COV\Powers\PlantControl\RootScale3a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	1
		BHVR	V_COV\Powers\PlantControl\RootScale4a.bhvr
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	25

	Event
		Type	Local
		At	Offset1
		altpiv	13
		BHVR	V_COV\Powers\PlantControl\RootScale.bhvr
		Geom	BriarVine01
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	2
		BHVR	V_COV\Powers\PlantControl\RootScale2.bhvr
		Geom	BriarVine
		Geom	BriarVineStraight
		Part	V_COV\Powers\PlantControl\TreeDustz.part
		Part	V_COV\Powers\PlantControl\TreeDust2z.part
	End
End

#############################################################

End