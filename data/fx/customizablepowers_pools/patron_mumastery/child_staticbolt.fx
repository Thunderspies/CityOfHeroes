#                                                          
FxInfo

	Input
		InpName Origin
	End

	Input
		InpName Target
	End

	Input
		InpName Mystic
	End

	Input
		InpName HandR
	End

	LifeSpan 60


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			Part :mu_Static_Redball1.part
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 15
		Chance 0.3

		Event
			At Hips
			Type PositOnly
			#Sound elecballhit2 100 30 0.2
		End

	End

	Condition
		On Time
		Time 20

		Event
			At t_root
			Type Start
			EName Grounder
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArcTarget.bhvr
			#Geom Testing_Target
		End

		Event
			At Origin
			Type Start
			EName Arc1
			LookAt Grounder
			Magnet Grounder
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArc_Start.bhvr
		End

		Event
			At Arc1
			Type Local
			EName Arc2
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_1.bhvr
		End

		Event
			At Arc1
			Type Local
			EName Arc3
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_2.bhvr
		End

		Event
			At Arc1
			Type Local
			EName Arc4
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArc_090_3.bhvr
		End

		Event
			At Arc1
			Type Local
			EName Arc5
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArc_end.bhvr
		End

		Event
			At arc1
			Type PositOnly
			EName walker1
			Magnet Arc2
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
			LifeSpan 5
		End

		Event
			At walker1
			Type Local
			PMagnet Root
			#POther arc1
			Part :mu_StaticArcWalker1.part
			LifeSpan 15
		End

		Event
			At Arc2
			Type PositOnly
			EName walker2
			Magnet Arc3
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
			LifeSpan 5
		End

		Event
			At walker2
			Type Local
			PMagnet Root
			#POther arc2
			Part :mu_StaticArcWalker1.part
			LifeSpan 15
		End

		Event
			At Arc3
			Type PositOnly
			EName walker3
			Magnet Arc4
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
			LifeSpan 5
		End

		Event
			At walker3
			Type Local
			PMagnet Root
			#POther arc2
			Part :mu_StaticArcWalker1.part
			LifeSpan 15
		End

		Event
			At Arc4
			Type PositOnly
			EName walker4
			Magnet Arc5
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
			LifeSpan 5
		End

		Event
			At walker4
			Type Local
			PMagnet Root
			#POther arc2
			Part :mu_StaticArcWalker1.part
			LifeSpan 15
		End

	End

End
