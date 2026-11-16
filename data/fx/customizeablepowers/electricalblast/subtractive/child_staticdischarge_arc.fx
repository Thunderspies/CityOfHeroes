#############################################################
## Child_StaticDischarge_Arc.fx
#############################################################

FxInfo

Lifespan 60

## HAND ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	wepL
		Part	:mu_Cage_HandArchs2.part
		Part	:mu_Cage_HandArchs_White1.part
		Part	:ElectricityRing.part
		Lifespan  5
	End
End

## OFFSETS ###########################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	Grounder
		Type	Start
		At	T_Root
		Bhvr	:mu_StaticArcTarget.Bhvr
	End

	Event
		Ename	Arc1
		Type	Start
		At	origin
		Bhvr	:mu_StaticArc_Start.Bhvr
		Magnet	Grounder
		LookAt	Grounder
	End

	Event
		Ename	Arc2
		Type	Local
		At	Arc1
		Bhvr	:mu_StaticArc_090_1.Bhvr
	End

	Event
		Ename	Arc3
		Type	Local
		At	Arc1
		Bhvr	:mu_StaticArc_090_2.Bhvr
	End

	Event
		Ename	Arc4
		Type	Local
		At	Arc1
		Bhvr	:mu_StaticArc_090_3.Bhvr
	End

	Event
		Ename	Arc5
		Type	Local
		At	Arc1
		Bhvr	:mu_StaticArc_End.Bhvr
	End
End

############################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	walker1
		Type	PositOnly
		At	arc1
		Bhvr	:mu_StaticArcWalker1.Bhvr
		Magnet	Arc2
		Lifespan 5
	End

	Event
		Type	Local
		At	walker1
		Part	:mu_StaticArcWalker_Invert1a.part
		Part	:mu_StaticArcWalker1a.part
		PMagnet	Root
		Lifespan 15
	End
End

############################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	walker2
		Type	PositOnly
		At	Arc2
		Bhvr	:mu_StaticArcWalker1.Bhvr
		Magnet	Arc3
		Lifespan 5
	End

	Event
		Type	Local
		At	walker2
		Part	:mu_StaticArcWalker_Invert1a.part
		Part	:mu_StaticArcWalker1a.part
		PMagnet	Root
		Lifespan 15
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	walker3
		Type	PositOnly
		At	Arc3
		Bhvr	:mu_StaticArcWalker1.Bhvr
		Magnet	Arc4
		Lifespan 5
	End

	Event
		Type	Local
		At	walker3
		Part	:mu_StaticArcWalker_Invert1a.part
		Part	:mu_StaticArcWalker1a.part
		PMagnet	Root
		Lifespan 15
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	walker4
		Type	PositOnly
		At	Arc4
		Bhvr	:mu_StaticArcWalker1.Bhvr
		Magnet	Arc5
		Lifespan 5
	End

	Event
		Type	Local
		At	walker4
		Part	:mu_StaticArcWalker_Invert1a.part
		Part	:mu_StaticArcWalker1a.part
		PMagnet	Root
		Lifespan 15
	End

End

#############################################################

End