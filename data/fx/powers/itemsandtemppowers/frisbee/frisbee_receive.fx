#
FxInfo

	LifeSpan 50


###########################################################

	Condition

		Event
			At Origin
			Type Local
			Sound SoccerBall_Catch_02 50 50 0.25
			Sound SoccerBall_Catch_05 50 50 0.25
		End

	End

	Condition
		On Time
		Time 4

		Event
			At WepR
			Type Local
			EName StartAnchor
			Bhvr :Frisbee_Death.bhvr
			BhvrOverride
				PositionOffset 0.5 -0.05 0
				Scale 0.12 0.12 0.12
			End
			Geom Frisbee
			LifeSpan 30
		End

	End

End
