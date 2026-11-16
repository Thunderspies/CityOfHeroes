#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Posit
			EName webcage
			Bhvr V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebCageSpin.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_netwebbingsmallCage
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName HeadFront
			Bhvr V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebCageFace.bhvr
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName HeadBack
			Bhvr V_COV\EnemyPowers\Arachnos\Arachnoid_B_WebCageBack.bhvr
		End

	End

	Condition
		On Time
		Time 1

		Event
			AltPiv 1
			At WebCage
			Type Start
			EName webcage
			POther HeadFront
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 6
			At WebCage
			Type Start
			EName webcage
			POther HeadBack
			Part :Arachnoid_P_WebCage.part
		End

		Event
			At Root
			Type Local
			EName webcage
			POther HeadBack
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			At Root
			Type Local
			EName webcage
			POther HeadFront
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 10
			At WebCage
			Type Start
			EName webcage
			POther Neck
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 9
			At WebCage
			Type Start
			EName webcage
			POther Chest
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 8
			At WebCage
			Type Start
			EName webcage
			POther Chest
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 7
			At WebCage
			Type Start
			EName webcage
			POther Chest
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 6
			At WebCage
			Type Start
			EName webcage
			POther Chest
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 5
			At WebCage
			Type Start
			EName webcage
			POther Chest
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 4
			At WebCage
			Type Start
			EName webcage
			POther Chest
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 3
			At WebCage
			Type Start
			EName webcage
			POther neck
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 1
			At WebCage
			Type Start
			EName webcage
			POther Hips
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 2
			At WebCage
			Type Start
			EName webcage
			POther Hips
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 3
			At WebCage
			Type Start
			EName webcage
			POther Hips
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 4
			At WebCage
			Type Start
			EName webcage
			POther Hips
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 5
			At WebCage
			Type Start
			EName webcage
			POther Hips
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 6
			At WebCage
			Type Start
			EName webcage
			POther Hips
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 7
			At WebCage
			Type Start
			EName webcage
			POther Hips
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 8
			At WebCage
			Type Start
			EName webcage
			POther Hips
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 6
			At WebCage
			Type Start
			EName webcage
			POther UarmR
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 6
			At WebCage
			Type Start
			EName webcage
			POther UarmR
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 2
			At WebCage
			Type Start
			EName webcage
			POther UarmL
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 2
			At WebCage
			Type Start
			EName webcage
			POther UarmL
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 4
			At WebCage
			Type Start
			EName webcage
			POther LarmL
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 4
			At WebCage
			Type Start
			EName webcage
			POther LarmL
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 9
			At WebCage
			Type Start
			EName webcage
			POther LarmR
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 9
			At WebCage
			Type Start
			EName webcage
			POther LarmR
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 4
			At WebCage
			Type Start
			EName webcage
			POther RingL
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 5
			At WebCage
			Type Start
			EName webcage
			POther RingL
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 10
			At WebCage
			Type Start
			EName webcage
			POther RingR
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 10
			At WebCage
			Type Start
			EName webcage
			POther RingR
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 1
			At WebCage
			Type Start
			EName webcage
			POther UlegL
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 1
			At WebCage
			Type Start
			EName webcage
			POther UlegL
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 6
			At WebCage
			Type Start
			EName webcage
			POther UlegR
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 6
			At WebCage
			Type Start
			EName webcage
			POther UlegR
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 7
			At WebCage
			Type Start
			EName webcage
			POther LlegL
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 7
			At WebCage
			Type Start
			EName webcage
			POther LlegL
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 2
			At WebCage
			Type Start
			EName webcage
			POther LlegR
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 2
			At WebCage
			Type Start
			EName webcage
			POther LlegR
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 3
			At WebCage
			Type Start
			EName webcage
			POther FootL
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 3
			At WebCage
			Type Start
			EName webcage
			POther FootL
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 8
			At WebCage
			Type Start
			EName webcage
			POther FootR
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 8
			At WebCage
			Type Start
			EName webcage
			POther FootR
			Part :Arachnoid_P_WebCage.part
		End

		Event
			AltPiv 4
			At WebCage
			Type Start
			EName webcage
			POther ToeR
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			AltPiv 9
			At WebCage
			Type Start
			EName webcage
			POther ToeR
			Part :Arachnoid_P_WebCage01.part
		End

		Event
			At Root
			Type Start
			EName lazer
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_Web1
		End

		Event
			At Root
			Type Start
			EName lazer
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_Web2
		End

		Event
			At Root
			Type Start
			EName lazer
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_Web3
		End

		Event
			At Root
			Type Start
			EName lazer
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_Web4
		End

	End

End
