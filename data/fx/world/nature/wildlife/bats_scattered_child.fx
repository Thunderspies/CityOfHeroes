#
FxInfo


###########################################################

	Condition
		On TimeOfDay
		DayStart 19.0000
		DayEnd 6.0000
		Domany 1

		Event
			At Origin
			Type Local
			EName BatAnchor
			Flags OneAtATime
			LifeSpan 200
		End

	End

	Condition
		On Time
		Time 0

		Event
			At BatAnchor
			Type Local
			EName RotatorAnchor
			BhvrOverride
				PyrRotateJitter 90 0 0
				Spin 0 0.1 0
				SpinJitter 0 0.015 0
				StartJitter 20 5 20
				TintGeom 1
				Rgb0 0 0 0
			End
		End

		Event
			Anim FX_Bat_01
			At RotatorAnchor
			Type Local
			EName BatGeo
			Bhvr behaviors/Scale_Bats.bhvr
			BhvrOverride
				EndScale 0.1 0.1 0.1
				PositionOffset 5 5 5
				PyrRotate 0 135 90
				Spin 0 0 0.1
				FadeOutLength 60
			End
			LifeSpan 80
		End

	End

End
