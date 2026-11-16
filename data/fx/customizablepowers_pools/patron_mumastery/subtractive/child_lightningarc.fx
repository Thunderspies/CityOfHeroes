#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 40


###########################################################

	Condition
		On Time
		Time 0

		Event
			At WepL
			Type Local
			EName base
			LifeSpan 40
		End

	End

	Condition
		On Cycle
		Time 2

		Event
			At WepL
			Type Start
			EName Charger1
			Magnet Chest
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_BlastChargers1.bhvr
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 10

		Event
			Type Destroy
			EName Charger1
		End

		Event
			Type Destroy
			EName ChargeArcs
		End

	End

	Condition
		On Time
		Time 0

		Event
			At WepL
			Type Posit
			EName Aim
			LookAt T_chest
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_ArcAim1.bhvr
			LifeSpan 40
		End

		Event
			At WepL
			Type Local
			EName RotSpinner
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_ArcRotSpinner1.bhvr
			BhvrOverride
				PositionOffset	0 0 -0.5
			End
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 16

		Event
			At RotSpinner
			Type Start
			EName Arc1
			Bhvr V_COV\EnemyPowers\Arachnos\Mu_BlastArc_Start.bhvr
			LifeSpan 40
		End

		Event
			At Arc1
			Type Local
			EName Arc2
			Bhvr V_COV\EnemyPowers\Arachnos\Mu_BlastArc_1.bhvr
		End

		Event
			At Arc1
			Type Local
			EName Arc3
			Bhvr V_COV\EnemyPowers\Arachnos\Mu_BlastArc_2.bhvr
		End

		Event
			At Arc1
			Type Local
			EName Arc4
			Bhvr V_COV\EnemyPowers\Arachnos\Mu_BlastArc_3.bhvr
		End

		Event
			At Arc1
			Type Local
			EName Arc5
			Bhvr V_COV\EnemyPowers\Arachnos\Mu_BlastArc_4.bhvr
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
			PMagnet T_Hips
			Part :mu_Elec_ArcLink.part
			Part :mu_Elec_ArcLinkCore.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 17

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
			PMagnet T_Hips
			Part :mu_Elec_ArcLink.part
			Part :mu_Elec_ArcLinkCore.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 18

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
			PMagnet T_Hips
			Part :mu_Elec_ArcLink.part
			Part :mu_Elec_ArcLinkCore.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 19

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
			PMagnet T_Hips
			Part :mu_Elec_ArcLink.part
			Part :mu_Elec_ArcLinkCore.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Arc5
			Type PositOnly
			EName walker5
			Magnet t_chest
			Bhvr V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
			LifeSpan 5
		End

		Event
			At walker5
			Type Local
			PMagnet T_Hips
			Part :mu_Elec_ArcLink.part
			Part :mu_Elec_ArcLinkCore.part
			LifeSpan 30
		End

	End

End
