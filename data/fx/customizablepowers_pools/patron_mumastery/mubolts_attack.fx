#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 26


###########################################################

	Condition
		On Time
		Time 22

		Event
			At WepL
			Type Posit
			Sound eleczapquick 60 60 0.5
		End

		Event
			At WepL
			Type Local
			EName origin
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_bolt_offset1.bhvr
		End

		Event
			At WepL
			Type Start
			EName aim
			LookAt Target
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_BoltAim1.bhvr
			#Geom Testing_Target
		End

		Event
			At aim
			Type Start
			EName walker
			Magnet Target
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_BoltWalker1.bhvr
			#Geom Testing_Target
		End

		Event
			At walker
			Type Local
			POther target
			Part CustomizablePowers_Pools\Patron_MuMastery\Mu_Elec_BoltLink1.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Mu_Elec_BoltLink1.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Mu_Elec_BoltLink1.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Mu_Elec_BoltLink1A.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Mu_Elec_BoltLink1A.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Mu_Elec_BoltLink1A.part
			LifeSpan 10
		End

		Event
			At walker
			Type Local
			Part CustomizablePowers_Pools\Patron_MuMastery\Mu_Elec_BoltCore1.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Mu_Elec_BoltCore2.part
			LifeSpan 10
		End

	End

End
