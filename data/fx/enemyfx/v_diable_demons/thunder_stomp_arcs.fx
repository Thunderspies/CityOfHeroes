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
		Ename	Grounder
		Type	local
		At	origin
	End

	Event
		Ename	Arc1
		Type	start
		At	handR
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArc_Start.bhvr
		magnet	Grounder
		LookAt	Grounder
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
		lifespan 5
		magnet	Arc2
	End

	Event
		Type	local
		At	walker1
		Part	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.part
		lifespan 15
		Pmagnet	Root
	End

	Event
		Ename	walker2
		Type	PositOnly
		At	Arc2
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		magnet	Arc3
	End

	Event
		Type	local
		At	walker2
		Part	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.part
		lifespan 15
		Pmagnet	Root
	End

	Event
		Ename	walker3
		Type	PositOnly
		At	Arc3
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		magnet	Arc4
	End

	Event
		Type	local
		At	walker3
		Part	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.part
		lifespan 15
		Pmagnet	Root
	End

	Event
		Ename	walker4
		Type	PositOnly
		At	Arc4
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		magnet	Arc5
	End

	Event
		Type	local
		At	walker4
		Part	ENEMYFX\V_Diable_Demons\Thunder_Stomp_Branches.part
		lifespan 15
		Pmagnet	Root
	End

	Event
		Type	startpositonly
		At	grounder
		Part	V_COV\EnemyPowers\Arachnos\mu_EMP_OrbSpots.part
		Part	V_COV\EnemyPowers\Arachnos\mu_EMP_OrbSpotsFront.part
		lifespan 1
	End
End

#############################################################

End