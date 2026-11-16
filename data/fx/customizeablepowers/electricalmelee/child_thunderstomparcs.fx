#############################################################
## Thunder_Stomp_Arcs.fx
#############################################################

FxInfo

Lifespan 11

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Ename	Arc1
		Type	start
		At	handR
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_Start.bhvr
		Magnet	Origin
		LookAt	Origin
	End

	Event
		Ename	Arc2
		Type	local
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_1.bhvr
	End

	Event
		Ename	Arc3
		Type	local
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_2.bhvr
	End

	Event
		Ename	Arc4
		Type	local
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_3.bhvr
	End

	Event
		Ename	Arc5
		Type	local
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_End.bhvr
	End

	Event
		Ename	walker1
		Type	PositOnly
		At	arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		Magnet	Arc2
		lifespan 5
	End

	Event
		Type	local
		At	walker1
		Part	:Thunder_Stomp_Branches.part
		PMagnet	Root
		lifespan 15
	End

	Event
		Ename	walker2
		Type	PositOnly
		At	Arc2
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		Magnet	Arc3
		lifespan 5
	End

	Event
		Type	local
		At	walker2
		Part	:Thunder_Stomp_Branches.part
		PMagnet	Root
		lifespan 15
	End

	Event
		Ename	walker3
		Type	PositOnly
		At	Arc3
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		Magnet	Arc4
		lifespan 5
	End

	Event
		Type	local
		At	walker3
		Part	:Thunder_Stomp_Branches.part
		PMagnet	Root
		lifespan 15
	End

	Event
		Ename	walker4
		Type	PositOnly
		At	Arc4
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		Magnet	Arc5
		lifespan 5
	End

	Event
		Type	local
		At	walker4
		Part	:Thunder_Stomp_Branches.part
		PMagnet	Root
		lifespan 15
	End
End

#############################################################

End