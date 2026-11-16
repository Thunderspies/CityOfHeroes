#
FxInfo

	LifeSpan 80


###########################################################

	Condition
		On Time
		Time 20

		Event
			At Head
			Type Local
			Sound steamblast2 100 100 0.75
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Head
			Type Local
			Sound waterfountain2_loop 100 100 0.66
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Head
			Type PositOnly
			EName header
			LookAt T_Root
		End

		Event
			At header
			Type Local
			EName Spray
			Bhvr V_COV\EnemyPowers\Coralax\WaterSprayOffset.bhvr
			Part :WaterFountainMedium.part
		End

		Event
			At spray
			Type Local
			BhvrOverride
				PositionOffset 0 0.05 0
			End
			Part :WaterSpray_Spittle.part
		End

	End

End
