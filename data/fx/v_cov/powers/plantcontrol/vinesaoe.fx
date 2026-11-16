#############################################################
## Plant Control - Vines
#############################################################

FxInfo

Lifespan	400

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Origin
		Sound tree_emerge 70 70 .7
	End

	Event
		Type	Local
		At	Origin
		Sound tar_loop 70 70 .5
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
		Bhvr	:RootPositionFade.bhvr
		Geom	HotFeet
		Sound summon2 90.0 90.0 .7
	End

	Event
		EName	Offset2
		Type	start
		At	T_Root
		Bhvr	:RootPositionFade2.bhvr
		Geom	HotFeet01
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
		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	3
		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	Offset1
		altpiv	2
		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	4
		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	5

	Event
		Type	Local
		At	Offset1
		altpiv	3
		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	5
		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	7

	Event
		Type	Local
		At	Offset1
		altpiv	4
		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	6
		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	9

	Event
		Type	Local
		At	Offset1
		altpiv	5
		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	7
		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	11

	Event
		Type	Local
		At	Offset1
		altpiv	6
		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	8
		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	13

	Event
		Type	Local
		At	Offset1
		altpiv	7
		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	9
		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	15

	Event
		Type	Local
		At	Offset1
		altpiv	8
		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	10
		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	17

	Event
		Type	Local
		At	Offset1
		altpiv	9
		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	11
		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	19

	Event
		Type	Local
		At	Offset1
		altpiv	10
		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	12
		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	21

	Event
		Type	Local
		At	Offset1
		altpiv	11
		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	13
		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	23

	Event
		Type	Local
		At	Offset1
		altpiv	12
		BHVR	:RootScale3.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	1
		BHVR	:RootScale4.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

Condition
	On 	time
	Time 	25

	Event
		Type	Local
		At	Offset1
		altpiv	13
		BHVR	:RootScale.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End

	Event
		Type	Local
		At	Offset2
		altpiv	2
		BHVR	:RootScale2.bhvr
		Geom	straightRoot01
		Geom	straightRoot
		Geom	straightRoot01Dark
		Geom	straightRootDark
		Part	:TreeDustz.part
		Part	:TreeDust2z.part
	End
End

#############################################################

End		