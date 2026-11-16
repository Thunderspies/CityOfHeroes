#                                                          
FxInfo

	Input
		InpName Chest
	End

	LifeSpan 40


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			EName groundspot
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_Hit_Groundspot.bhvr
			#Geom Testing_Target
		End

		Event
			At Chest
			Type Posit
			EName groundwalker
			Magnet groundspot
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_Hit_Grounder.bhvr
			#Geom Testing_Target
		End

		Event
			At groundwalker
			Type Posit
			#Magnet Root
			Part :mu_Elec_Grounder.part
			Part :mu_Elec_Grounder2.part
			#Sound lightningbolthit 100 30 0.7
			LifeSpan 40
		End

		Event
			At Root
			Type Start
			EName groundspot2
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_Hit_Groundspot.bhvr
			#Geom Testing_Target
		End

		Event
			At Head
			Type Posit
			EName groundwalker2
			Magnet groundspot2
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_Hit_Grounder.bhvr
			#Geom Testing_Target
		End

		Event
			At groundwalker2
			Type Posit
			#Magnet Root
			Part :mu_Elec_Grounder.part
			Part :mu_Elec_Grounder.part
			#Sound lightningbolthit 100 30 0.7
			LifeSpan 40
		End

		Event
			At Root
			Type Start
			EName groundspot3
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_Hit_Grounder.bhvr
			#Geom Testing_Target
		End

		Event
			At Hips
			Type Posit
			EName groundwalker3
			Magnet groundspot3
			Bhvr V_COV\EnemyPowers\Arachnos\mu_Elec_Hit_Grounder.bhvr
			#Geom Testing_Target
		End

		Event
			At groundwalker3
			Type Posit
			#Magnet Root
			Part :mu_Elec_Grounder.part
			Part :mu_Elec_Grounder.part
			#Sound lightningbolthit 100 30 0.7
			LifeSpan 40
		End

	End

End
